
#include <shell.h>
#include <path.h>
#include <jobs.h>

int main(int argc, char *argv[], char *envp[]){
    StringList argumentos, comandos;
    String entrada;

    job_list = new_job_list();

    if(fork() == 0){
        execv("/usr/bin/clear", argv);
    }else{
        wait(NULL);
    }
    do{
        printf("A7> ");

        // remove filhos da lista de jobs
        if(equals((entrada = get_line()), "quit")){
            kill_shell();
        }
        if(entrada)
            if(entrada[0] == '\0')
                continue;
        argumentos = explode(entrada, ' ');
        trim_string_list(&argumentos, ' ');
        // pega o primeiro argumento
        //comandos = path_concat(envp, get_string(argumentos, 0));

        //faz verificação de redirecionamento de E/S 
        int z = verifica_redirecionamento(&argumentos);

        if(executa_comandos_internos(argumentos)) {
            normaliza_std(z); 
            continue;
        }           
        comandos = path_concat(envp, get_string(argumentos, 0));
        if(!executa_comandos(comandos, argumentos)){
            // trata erro de execucao
            printf("%s: command not found\n", get_string(argumentos, 0)); 
        }else{
            normaliza_std(z); 
            // faz fork?
        }
    }while(1); 
    return 0;
}
