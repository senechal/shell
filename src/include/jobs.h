#ifndef JOBS_H
#define JOBS_H

#include <string_list.h>
#include "mystring.h"
#include <sys/types.h>
#include <list.h>
#include <sys/procfs.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> 

typedef enum{
    STOPPED,
    RUNNING,
    ZOMBIE
} Status;

typedef struct job{
    pid_t jid;
    String pid;
    Status status;
    String name;
}_Job;

typedef _Job *Job;

Job new_job(String pid, String name);

void dispose_job(Job job);

Vector new_job_list();

void dispose_job_list(Vector job_list);

void job_list_push(Vector job_list, Job element);

Job get_current_job(Vector job_list);

Job get_at_index(Vector job_list, int pid);

Job job_list_next(Vector job_list);

void print_job_list(Vector job_list);

String get_status(Job job); 

String get_job_name(Job job);



Job get_by_pid(Vector vector, pid_t pid);

Job get_by_jid(Vector vector, pid_t jid);

int find_by_pid(Vector vector, pid_t pid);

int find_by_jid(Vector vector, pid_t jid);

void* remove_by_pid(Vector vector, pid_t pid);

void* remove_by_jid(Vector vector, pid_t jid);

String parse_proc_name(String name);

#endif
