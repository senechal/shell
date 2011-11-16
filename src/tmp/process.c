#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>


int main(int argc, char *argv[], char *evnp[]){
    char comando[] = "/bin/ls";
    char **argumentos;
    int nro_argumentos;
    int i;
    argumentos = (char**) malloc(sizeof(char*)*4);
    for(i = 0; i < 4; i++){
        argumentos[i] = (char*) malloc(sizeof(char)*10);
    }
    strcpy(argumentos[0], comando);
    strcpy(argumentos[1], "-a");
    strcpy(argumentos[2], "--color");
    strcpy(argumentos[3], "-l");
    if(fork() != 0){
    }else{
    }
    return 0;
}
