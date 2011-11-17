#include "jobs.h"

Job new_job(String pid, String name){
    static int counter = 1;
    Job job = (Job) malloc(sizeof(Job));
    job->jid = counter++;
    job->pid = copy_string(pid);
    job->status = RUNNING;
    job->name = copy_string(name);
    return job;
}

void dispose_job(Job job){
    dispose_string(&job->pid);
    free(job);
}

Vector new_job_list(){
    return new_vector();
}

void dispose_job_list(Vector job_list){
    dispose_vector(job_list);
}

void job_list_push(Vector job_list, Job element){
    vector_push(job_list, (void*) element);
}

Job job_list_next(Vector job_list){
    return (Job) vector_next(job_list);
}

Job get_current_job(Vector job_list){
    return (Job) get_current(job_list);
}

void print_job_list(Vector job_list){
    Job job;
    job_list->index = 0;
    String status;
    if(job_list->size > 0) {
    	while((job = job_list_next(job_list))){
        	status = get_status(job);
        	printf("%s\t%s\n", get_status(job), job->name);
	}
    }
    else printf ("No jobs running.\n");
}

Job get_at_index(Vector job_list, int pid) {
   Job temp;
   job_list->index = 0;
   temp = vector_next(job_list);
   while (((int)temp->pid != pid) && (temp->jid != pid) && (temp != NULL))
        temp = vector_next(job_list);
   return temp;
}

String get_status(Job job) {
    switch(job->status){
        case RUNNING:
            return copy_string("Running");
        case STOPPED:
            return copy_string("Stopped");
        case ZOMBIE:
            return copy_string("Zombie");
    }
}

String get_job_name(Job job){
    FILE *fd;
    prpsinfo_t retval; 
    String proc_str = copy_string("/proc/");
    append_string(&proc_str, job->pid);
    append_string(&proc_str, "/stat");
    fd = fopen(proc_str, "r");
    if(!fd){
        return copy_string("DEADPROC=P");  
    }
    if(!fread(&retval, sizeof(retval), 1, fd)){
        return copy_string("Error: Invalid State, the return");  
    }
    fclose(fd);
    return copy_string(retval.pr_fname);
}

Job get_by_pid(Vector vector, pid_t pid){
    int i;
    Job aux = NULL;
    for (i = 0; i < vector->size; i++) {
        aux = (Job) vector->elements[i];
        if(aux){
            if(string_to_int(aux->pid) == pid)
                return aux;
        }
    }
    return NULL;
}

Job get_by_jid(Vector vector, pid_t jid){
    int i;
    Job aux = NULL;
    for (i = 0; i < vector->size; i++) {
        aux = (Job) vector->elements[i];
        if(aux){
            if(aux->jid == jid)
                return aux;
        }
    }
    return NULL;
}

int find_by_pid(Vector vector, pid_t pid){
    int i;
    Job aux = NULL;
    for (i = 0; i < vector->size; i++) {
        aux = (Job) vector->elements[i];
        if(aux){
            if(string_to_int(aux->pid) == pid)
                return i;
        }
    }
    return -1;
}

int find_by_jid(Vector vector, pid_t jid){
    int i;
    Job aux = NULL;
    for (i = 0; i < vector->size; i++) {
        aux = (Job) vector->elements[i];
        if(aux){
            if(aux->jid == jid)
                return i;
        }
    }
    return -1;
}

void* remove_by_pid(Vector vector, pid_t pid){
    return vector_remove(vector, find_by_pid(vector, pid));
}

void* remove_by_jid(Vector vector, pid_t jid){
    return vector_remove(vector, find_by_jid(vector, jid));
}

String parse_proc_name(String name){
    int i;
    int size = size_of(name);
    int found = -1;
    String buffer = new_string();
    for(i = 0; i < size; i++){
        append(&buffer, name[i]);
        if(name[i] == '/'){
            found = i;
            dispose_string(&buffer);
            buffer = new_string();
        }
    }
    if(found == -1)
        return name;
    return buffer;
}
