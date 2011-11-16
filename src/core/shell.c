#include<shell.h>
#include<unistd.h>
#include<jobs.h>
#include <fcntl.h>
#include <stdio.h>

//controladores
//handler para tratar CTRL + C
static void kct_hand(int signo, siginfo_t *info, void *data) {

    if (background == 1) {
        return; 
    }
    else {
        Job job = vector_end(job_list);
        if (job)
            kill(string_to_int(job->pid), SIGINT);         
    }
}

//handler para tratar CTRL + Z
static void zct_hand(int signo, siginfo_t *info, void *data) {

    if(background == 0){
        vector_reset(job_list);
        Job job = (Job) vector_end(job_list);
        if(job){
            background = 1; 
            job->status = STOPPED;
            //kill(string_to_int(job->pid), SIGTSTP);
            kill(-getpid(), SIGTSTP);
        }
    }
}

//handler para tratar SIGCHLD
static void act_hand(int signo, siginfo_t *info, void *data) {

    waitpid(info->si_pid, NULL, 0); 
    vector_remove(job_list, find_by_pid(job_list, info->si_pid)); 
}

int try_execute(String comando, StringList argumentos){
    int c_pid;

    //handlers dos controladores'
    struct sigaction act; 
    struct sigaction kct; 
    struct sigaction zct; 

    act.sa_flags = SA_SIGINFO; 
    act.sa_sigaction = act_hand; 

    kct.sa_flags = SA_SIGINFO; 
    kct.sa_sigaction = kct_hand; 

    zct.sa_flags = SA_SIGINFO; 
    zct.sa_sigaction = zct_hand; 

    /*sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);

    sigprocmask(SIG_BLOCK, &mask, NULL); */

    c_pid = fork();
    if (c_pid == 0) {
        //processo filho
        setpgid(0, 0);
//        sigprocmask(SIG_UNBLOCK, &mask, NULL); 
        execv(comando, to_array(argumentos));
    } else {
        //processo pai

        //controle sobre o sinal SIGCHLD, enviado pelo processo filho após
        //terminar sua execução 
        sigaction(SIGCHLD, &act, 0); 

        //Controle do sinal do CTRL + C
        sigaction(SIGINT, &kct, 0); 

        //Controle do sinal do CTRL + Z
        sigaction(SIGTSTP, &zct, 0); 

        //adiciona na lista de jobs
        job_list_push(job_list, new_job(copy_string(int_to_string(c_pid)), parse_proc_name(comando)));
        if(background == 1){ 
            return 1;
        }else{
            //Job job = (Job) vector_end(job_list); 
            //waitpid(string_to_int(job->pid), NULL, 0); 
            wait(NULL); 
        }
    }
    return 1;
}

int executa_comandos(StringList comandos, StringList argumentos){
    FILE *file;
    int ret = 0;
    background = 0;
    String comando = new_string();
    int position;
    String tmp;
    if(list_find_char_at_last(comandos, '&') != -1){
        while((tmp = list_next(&comandos))){
            remove_last_char(&tmp);
        }
        background = 1;
    }if((position = list_find_char_at_last(argumentos, '&'))){
        if(position != -1){
            remove_last_char(&argumentos.string[position]);
            background = 1;
        }
    }
    reset_index(&comandos);
    while((comando = list_next(&comandos))){
        file = fopen(comando, "r");
        if(!file)
            continue;
        fclose(file);
        ret = try_execute(comando, argumentos);
    }
    dispose_string(&comando);
    return ret;
}

//verifica se é necessário realizar redirecionamento de E/S e retorna um inteiro
//indicando a ação realizada pela função
//0 -> não fez nada
//1 -> redirecionou stdout (sem append)
//2 -> redirecionou stdout (com append)
//3 -> redirecionou stderr 
//4 -> redirecionou stdin
int verifica_redirecionamento(StringList *argumentos) {

    int ret = 0;
    String file = new_string();
    file = copy_string(get_string(*argumentos, ((*argumentos).size -1)));

    int fd;

    if (equals(get_string(*argumentos, ((*argumentos).size - 2)), ">")) {
        fd = open(file,  O_WRONLY | O_CREAT); 
        dup2(fd,1);
        ret = 1;
    }
    else if (equals(get_string(*argumentos, (*argumentos).size - 2), ">>")) {
        fd = open(file, O_APPEND | O_WRONLY | O_CREAT);
        dup2(fd,1);
        ret = 2;
    }
    else if (equals(get_string(*argumentos, (*argumentos).size - 2), "2>")) {
        fd = open(file, O_APPEND | O_WRONLY | O_CREAT);
        dup2(fd,2);
        ret = 3;
    }
    else if (equals(get_string(*argumentos, (*argumentos).size - 2), "<")) {
        fd = open(file, O_APPEND | O_WRONLY | O_CREAT);
        dup2(fd,0);
        ret = 4;
    }

    if (ret != 0) {
        list_remove(argumentos,(*argumentos).size-2);
        list_remove(argumentos,(*argumentos).size-1);
    }

    return ret; 
}

void normaliza_std(int ret) {
    int fd;
    if ((ret == 1) || (ret == 2)){
        fd = open("/dev/tty", O_WRONLY);
        dup2(fd, 1);  
    }
    else if (ret == 3){
        fd = open("/dev/tty", O_RDONLY | O_WRONLY);
        dup2(fd, 2); 
    }

    else if (ret == 4){
        fd = open("/dev/tty", O_RDONLY);
        dup2(fd, 0); 
    }
}

int executa_comandos_internos(StringList argumentos){
    String comando = get_string(argumentos, 0);

    if(equals(comando, "cd")){
        if(chdir(get_string(argumentos, 1))){
            printf("Error: no such directory\n");
            return 0;
        }
    }else if(equals(comando, "pwd")){
        printf("%s\n", get_current_dir_name());
    }else if(equals(comando, "jobs")){
        print_job_list(job_list);
    }else if(equals(comando,"fg")){
        Job aux; 
        int pid = string_to_int(get_string(argumentos, 1)); 
        kill(pid, SIGCONT);
        aux = get_by_pid(job_list, pid); 
        if (aux == NULL) {
           //busca por jid 
           aux = get_by_jid(job_list, pid); 
           if (aux == NULL) {
                printf("Error: job not found\n"); 
                return 0; 
           }
        }
        aux->status = RUNNING; 
        background = 0;
        wait(NULL); 
    }else if(equals(comando,"bg")){
        Job aux; 
        int pid = string_to_int(get_string(argumentos, 1)); 
        kill(pid ,SIGCONT);
        aux = get_by_pid(job_list, pid); 
        if (aux == NULL) {
           //busca por jid 
           aux = get_by_jid(job_list, pid); 
           if (aux == NULL) {
                printf("Error: job not found\n"); 
                return 0; 
           }
        }
        aux->status = RUNNING;
        background = 1; 
    }else{
        return 0;
    }
    return 1;
}

String get_line(){
    char c;
    int saida;
    String string = new_string();
    string = copy_string("");
    saida = scanf(" %c", &c);
    while (c != '\n') {
        if ((saida == EOF) && (size_of(string) > 0))
            return copy_string("quit"); 
        if (saida != 0)
            append(&string, c);
        saida = scanf("%c", &c); 
    }
    append(&string, '\0');
    return string; 
}

void kill_shell(){
    // mata todos os seus jobs e termina a execucao
    Job job;
    int pid;
    vector_reset(job_list);
    while((job = vector_next(job_list))){
        pid = string_to_int(job->pid);
        kill(pid, SIGKILL);
    }
    printf("\n");
    exit(0);
}


