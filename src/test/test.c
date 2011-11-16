#include <stdio.h>
#include <list.h>
#include <jobs.h>
#include <mystring.h>

int main(int argc, const char *argv[])
{
    Job job = new_job("1200");
    Vector vector = new_vector();
    vector_push(vector, job);
    job = new_job("1300");
    vector_push(vector, job);
    Job job2 = vector_end(vector);
    printf("%s\n", job2->pid);
    vector_reset(vector);
    job2 = get_current(vector);
    printf("%s\n", job2->pid);
    dispose_vector(vector);
    return 0;
}
