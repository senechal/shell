#ifndef SHELL_H
#define SHELL_H

#include <mystring.h>
#include <string_list.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <list.h>

int background; 

Vector job_list;

static void act_hand(int signo, siginfo_t *info, void *data); 

static void kct_hand(int signo, siginfo_t *info, void *data); 

static void zct_hand(int signo, siginfo_t *info, void *data); 


void block(sigset_t *mask, sigset_t *old_mask); 

void unblock(sigset_t *mask, sigset_t *old_mask); 


int try_execute(String comando, StringList argumentos);

int verifica_redirecionamento(StringList *argumentos); 

int executa_comandos(StringList comandos, StringList argumentos);

String get_line();

void kill_shell(/*Shell shell*/);

int executa_comandos_internos(StringList argumentos);

String get_pwd(char * env[]);

void normaliza_std(int ret);

#endif
