/**
 *
 * Trabalho - Sistemas operacionais II - Shell
 *
 * Alunos:
 *  Guilherme Takasawa Yagui - 6426698
 *  Andre Luiz Lé Sénéchal Paolino  - 5886766
 *
 */

#include "shell.h"
#include "path.h"
#include "jobs.h"

int main(int argc, char *argv[], char *envp[]){
    StringList arguments, commands;
    String entry;

    job_list = new_job_list();

    if(fork() == 0){
        execv("/usr/bin/clear", argv);
    }else{
        wait(NULL);
    }
    do{
        printf("Shell> ");

        // remove filhos da lista de jobs
        if(equals((entry = get_line()), "exit")){
            kill_shell();
        }
        if(entry)
            if(entry[0] == '\0')
                continue;
	//quebra a linha de entrada em comandos em uma lista de string depois remove eventuis strings vazias
        arguments = explode(entry, ' ');
        trim_string_list(&arguments, ' ');

        //faz verificação de redirecionamento de E/S 
        int z = verify_redirect(&arguments);
	//executa comandos na lista arguments
        if(execute_int_commands(arguments)) {
            normalize_std(z); 
            continue;
        }
	// cria o PATH do comando      
        commands = path_concat(envp, get_string(arguments, 0));
	//executa os comandos de acordo com o PATH
        if(!execute_commands(commands, arguments)){
            // trata erro de execucao
            printf("%s: command not found\n", get_string(arguments, 0)); 
        }else{
            normalize_std(z); 
        }
    }while(1); 
    return 0;
}
