
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

#define MAX_SIZE_IN 255
#define ABSOLUTE 1
#define RELATIVE 0
// Process functions

// Parser functions
void parse_entrada(char conteudo[], char *comando[], char **argumentos[], int *nro_argumentos){
    *nro_argumentos = 1;
    int i = 0,j = 0,l = 0, k = 0, s = 0;
    char temp[255];
    char *aux = NULL;
    // estado = 0 -> comando ; estado = 1 -> argumentos
    int estado = 0;
    int tamanho = 0;
    j = strlen(conteudo);
    *argumentos = (char**) malloc(sizeof(char*)*((*nro_argumentos)+1));
    (*argumentos)[0] = (char*) malloc(sizeof(char));
    (*argumentos)[0] = '\0';
    for (i = 0; i < j+1; i++) {
            if ((conteudo[i] == ' ') || (conteudo[i] == '\0')) {
                if (tamanho != 0 ) {
                    temp[l] = '\0';
                    if (estado == 0) {
                        *comando = (char *)malloc((tamanho+1)*(sizeof(char)));
                        (*argumentos) = (char**) malloc(sizeof(char*)*(*nro_argumentos));
                        strcpy(*comando,temp);
                        estado = 1;
                    }
                    else if (estado == 1) {
                        if (*nro_argumentos == 1){
                        }
                        *argumentos = (char**) realloc(*argumentos, sizeof(char*)*((*nro_argumentos)+1));
                        while (temp[k] != '\0') {
                            aux = (char*) realloc(aux, sizeof(char)*(s+1));
                            aux[s] = temp[k];
                            s++;
                            k++;
                        }
                        aux[s] = '\0';
                        (*argumentos)[(*nro_argumentos)] = (char*) malloc(sizeof(char)*(strlen(aux)+1));
                        strcpy((*argumentos)[(*nro_argumentos)], aux);
                        if(aux)
                            free(aux);
                        (*nro_argumentos)++;
                        k = 0;
                        s = 0;

                    }
                    tamanho = 0;
                    l = 0;
                    estado = 1;
                    temp[0] = '\0';
                } else continue;
            }
            else {
                temp[l] = conteudo[i];
                tamanho = tamanho + 1;
                l++;
            }
    }
    (*argumentos) = (char**) realloc((*argumentos), sizeof(char*)*(*nro_argumentos+1));
    (*argumentos)[*nro_argumentos] = NULL;
}

//impressao
void print(int nro_argumentos, char **argumentos, char *comando){
    int i, s = 0;
    printf("%s\n",comando);  
    for (i = 1; i < (nro_argumentos); i++) {
        while (argumentos[i][s] != '\0') {
            printf("%c",argumentos[i][s]);
            s++;
        }
        s = 0;
    }
}

char * get_path(char *env[]){
    int i = 0, j;
    char tmp[5];
    char * path;
    while(env[i]!=NULL){
        strncpy(tmp, env[i], 4);
        tmp[4] = '\0';
        if(strcmp(tmp,"PATH") == 0){
            path = (char *) malloc(sizeof(char)*(strlen(env[i]) - 4));
            for(j = 5; j < strlen(env[i]); j++){
                path[j-5] = env[i][j];
            }
            path[strlen(env[i]) - 5] = '\0';
            return path;
        }
        i++;
    }
    return NULL;
}

char ** get_path_array(char *env[]){
    char * tmp = get_path(env);
    char ** path_array = NULL;
    char *aux = NULL;
    int i = 0, j = 0, size = 1;
    while(tmp[size-1] != '\0'){
        path_array = (char**) realloc(path_array, sizeof(char*)*(i+1));
        while(tmp[size-1] != ':' && tmp[size-1] != '\0'){
            aux = (char*) realloc(aux, sizeof(char)*(j+1));
            aux[j] = tmp[size-1];
            j++;
            size++;
        }
        aux[j] = '\0';
        path_array[i] = (char*) malloc(sizeof(char)*(size));
        strcpy(path_array[i], aux);
        if(aux)
            free(aux);
        aux = NULL;
        j = 0;
        i++;
        if(tmp[size-1] == '\0')
            break;
        size++;
    }
    // adiciona-se uma posicao nula no vetor de strings para delimitar o fim
    path_array = (char**) realloc(path_array, sizeof(char*)*(i+1));
    path_array[i] = NULL;
    if(tmp){
        free(tmp);
    }
    return path_array;
}

int try_execute(char *comando, char ***argumentos, int nro_argumentos){
    if (fork() == 0) {
        //processo filho
        execv(comando, (*argumentos));
    }
    else {
        //processo pai
        wait(NULL);
    }
    return 1;
}

int executa_comandos(char ***comandos, char ***argumentos, int nro_argumentos){
    int i = 0;
    int ret = 0;
    FILE *a; 
    while((*comandos)[i] != NULL){
        a = fopen((*comandos)[i], "r");
        if(a != NULL){
            fclose(a); 
            (*argumentos)[0] = (char*) malloc(sizeof(char)*(strlen((*comandos)[i])+1));
            strcpy((*argumentos)[0], (*comandos)[i]);
            ret = try_execute((*comandos)[i], argumentos, nro_argumentos);
            if ((*argumentos)[0])
                free((*argumentos)[0]); 
            break;
        }
        i++;
    }
    i = 0;
    while((*comandos)[i] != NULL){
        if((*comandos)[i])
            free((*comandos)[i]);
        i++;
    }
    if(*comandos)
        free(*comandos);
    i = 1;
    while((*argumentos)[i] != NULL){
        if((*argumentos)[i]) {
            free((*argumentos)[i]);
        }
        i++;
    }
    if(*argumentos)
        free(*argumentos);
    return ret;
}

char ** path_concat(char **path_array, char *comando){
    int i = 0, size = 0;
    char **new_path = NULL;
    while((path_array)[i] != NULL){
        i++;
    }
    size = i;
    new_path = (char**) malloc(sizeof(char*)*(size+1));
    if(!new_path){
        printf("Erro de alocacao de memoria 1 \n");
        exit(-1);
    }
    for(i = 0; i < size; i++){
        new_path[i] = (char*) malloc(sizeof(char)*(strlen((path_array)[i])+strlen(comando)+2));
        if(!new_path[i]){
            printf("Erro de alocacao de memoria 2\n");
            exit(-1);
        }
        strcpy(new_path[i], (path_array)[i]);
        strcat(new_path[i], "/");
        strcat(new_path[i], comando);
    }
    size = i;
    new_path[size] = NULL;
    i = 0;
    return new_path;
}

// Retorna 1 se caminho eh absoluto e 0 se relativo
int verifica_caminho(char *comando){
    // por ora, verifica apenas se o primeiro carcater é '/'
    if(comando[0] == '/')
        return ABSOLUTE; // absoluto
    return RELATIVE; // relativo
}

char **lista_comandos(char *comando, char *env[]){
    char **lista;
    char **path_array;
    int i = 0;
    if(verifica_caminho(comando) == ABSOLUTE){
        // absoluto retorna vetor com apenas um elemento
        lista = (char **) malloc(sizeof(char*)*2);
        lista[0] = (char*) malloc(sizeof(char)*strlen(comando));
        strcpy(lista[0], comando);
        lista[1] = NULL;
        return lista;
    }else{
        path_array = get_path_array(env);
        lista = path_concat(path_array, comando);
        while(path_array[i] != NULL){
            if(path_array[i])
                free(path_array[i]);
            i++;
        }
        if(path_array)
            free(path_array);
        return lista;
    }
}

//le entrada
int get_line(char **entrada) {
    char c; 
    int i = 0, saida = 255 ;
    (*entrada) = (char*) malloc(sizeof(char));
    saida = scanf("%c", &c); 
    while (c != '\n') {
        if (saida == EOF)
            return -1; 
        (*entrada) = (char*) realloc((*entrada), ++i);
        (*entrada)[i-1] = c; 
        saida = scanf("%c", &c); 
    }
    (*entrada) = (char*) realloc((*entrada), i+1); 
    (*entrada)[i] = '\0';
    return 1; 
}

int main(int argc, char *argv[], char *envp[]){
    char *comando, **argumentos, **comandos;
    int nro_argumentos;
    char *conteudo;
    // Executa o clear para limpar a tela
    if(fork() == 0){
        execv("/usr/bin/clear", argv);
    }else{
        wait(NULL);
    }
    do{
        printf("A7> ");
        if(get_line(&conteudo) == -1)
            return 1;
        if(conteudo)
            if(conteudo[0] == '\0')
                continue;
        parse_entrada(conteudo, &comando, &argumentos, &nro_argumentos);
        comandos = lista_comandos(comando, envp);
        if(!executa_comandos(&comandos, &argumentos, nro_argumentos)){
            // trata erro de execucao
            printf("Error: command not found\n"); 
        }
        if(comando)
            free(comando);
        if(conteudo)
            free(conteudo);
    }while(1); 
    return 0;
}
