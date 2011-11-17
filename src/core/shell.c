#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "shell.h"
#include "jobs.h"
#include "mystring.h"

//handler para tratar CTRL + C
static void ctrlc_hand(int signo, siginfo_t *info, void *data) {

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
static void ctrlz_hand(int signo, siginfo_t *info, void *data) {

    if(background == 0){
        vector_reset(job_list);
        Job job = (Job) vector_end(job_list);
        if(job){
            background = 1; 
            job->status = STOPPED;
            kill(-getpid(), SIGTSTP);
        }
    }
}

//handler para tratar SIGCHLD
static void sigchld_hand(int signo, siginfo_t *info, void *data) {

    waitpid(info->si_pid, NULL, 0); 
    vector_remove(job_list, find_by_pid(job_list, info->si_pid)); 
}

int try_execute(String command, StringList arguments){
    int c_pid;

    //handlers dos controladores
    struct sigaction sigchld; 
    struct sigaction ctrlc; 
    struct sigaction ctrlz; 

    sigchld.sa_flags = SA_SIGINFO; 
    sigchld.sa_sigaction = sigchld_hand; 

    ctrlc.sa_flags = SA_SIGINFO; 
    ctrlc.sa_sigaction = ctrlc_hand; 

    ctrlz.sa_flags = SA_SIGINFO; 
    ctrlz.sa_sigaction = ctrlz_hand; 

    c_pid = fork();
    if (c_pid == 0) {
        //processo filho
        setpgid(0, 0);
        execv(command, to_array(arguments));
    } else {
        /*processo pai
        //tem controle sobre o sinal SIGCHLD, enviado pelo processo filho apos terminar sua execucao 

	  controle dos sinais:
	*/
        sigaction(SIGCHLD, &sigchld, 0); 
        sigaction(SIGINT, &ctrlc, 0); 
        sigaction(SIGTSTP, &ctrlz, 0); 

        //adiciona na lista de jobs
        job_list_push(job_list, new_job(copy_string(int_to_string(c_pid)), parse_proc_name(command)));
        if(background == 1){ 
            return 1;
        }else{
            wait(NULL); 
        }
    }
    return 1;
}

int execute_commands(StringList commands, StringList arguments){
    FILE *file;
    int ret = 0;
    background = 0;
    String command = new_string();
    int position;
    String tmp;
	//verifica se o ultimo caracter do comand PATH é & se for remove e seta flag background pra 1
    if(list_find_char_at_last(commands, '&') != -1){
        while((tmp = list_next(&commands))){
            remove_last_char(&tmp);
        }
        background = 1;
	//verifica se omultimo caractere dos comandos é & se sim remove e seta a flag background pra 1
    }if((position = list_find_char_at_last(arguments, '&'))){
        if(position != -1){
            remove_last_char(&arguments.string[position]);
            background = 1;
        }
    }
    reset_index(&commands);
	// abre o arquivo a partir do comand PATH, se encontrar o arquivo tenta executar o comando
    while((command = list_next(&commands))){
        file = fopen(command, "r");
        if(!file)
            continue;
        fclose(file);
        ret = try_execute(command, arguments);
    }
    dispose_string(&command);
    return ret;
}

/*verifica se é necessário realizar redirecionamento de E/S e retorna um inteiro indicando a ação realizada pela função
  0 -> não fez nada
  1 -> redirecionou stdout (sem append)
  2 -> redirecionou stdout (com append)
  3 -> redirecionou stderr 
  4 -> redirecionou stdin
*/
int verify_redirect(StringList *arguments) {

    int ret = 0;
    String file = new_string();
    file = copy_string(get_string(*arguments, ((*arguments).size -1)));

    int fd;

    if (equals(get_string(*arguments, ((*arguments).size - 2)), ">")) {
        fd = open(file,  O_WRONLY | O_CREAT); 
        dup2(fd,1);
        ret = 1;
    }
    else if (equals(get_string(*arguments, (*arguments).size - 2), ">>")) {
        fd = open(file, O_APPEND | O_WRONLY | O_CREAT);
        dup2(fd,1);
        ret = 2;
    }
    else if (equals(get_string(*arguments, (*arguments).size - 2), "2>")) {
        fd = open(file, O_APPEND | O_WRONLY | O_CREAT);
        dup2(fd,2);
        ret = 3;
    }
    else if (equals(get_string(*arguments, (*arguments).size - 2), "<")) {
        fd = open(file, O_APPEND | O_WRONLY | O_CREAT);
        dup2(fd,0);
        ret = 4;
    }

    if (ret != 0) {
        list_remove(arguments,(*arguments).size-2);
        list_remove(arguments,(*arguments).size-1);
    }

    return ret; 
}

void normalize_std(int ret) {
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
//executa alguns comandos como cd e pwd...
int execute_int_commands(StringList arguments){
    String command = get_string(arguments, 0);

    if(equals(command, "cd")){
        if(chdir(get_string(arguments, 1))){
            printf("Error: no such directory\n");
            return 0;
        }
    }else if(equals(command, "pwd")){
        printf("%s\n", get_current_dir_name());
    }else if(equals(command, "jobs")){
        print_job_list(job_list);
    }else if(equals(command,"fg")){
        Job aux; 
        int pid = string_to_int(get_string(arguments, 1)); 
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
    }else if(equals(command,"bg")){
        Job aux; 
        int pid = string_to_int(get_string(arguments, 1)); 
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
            return copy_string("exit"); 
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


