#ifndef SHELL_H
#define SHELL_H

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "list.h"
#include "mystring.h"
#include "string_list.h"

int background; 

Vector job_list;

static void sigchld_hand(int signo, siginfo_t *info, void *data); 

static void ctrlc_hand(int signo, siginfo_t *info, void *data); 

static void ctrlz_hand(int signo, siginfo_t *info, void *data); 


void block(sigset_t *mask, sigset_t *old_mask); 

void unblock(sigset_t *mask, sigset_t *old_mask); 


int try_execute(String command, StringList arguments);

int verify_redirect(StringList *argumentos); 

int execute_commands(StringList commands, StringList arguments);

String get_line();

void kill_shell(/*Shell shell*/);

int execute_int_commands(StringList argumentos);

String get_pwd(char * env[]);

void normalize_std(int ret);

#endif
