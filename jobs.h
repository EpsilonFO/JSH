#ifndef JOBS_H
#define JOBS_H

#include <sys/types.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "prompt.h"


typedef struct jobs{
    char *name;
    int num_job;
    char *status;
    pid_t pid;
    int id_group_proc;
}jobs;

extern void add_job(jobs *, jobs *[]);
extern void remove_job(jobs *, jobs *[], prompt *);
extern void list_jobs(jobs *[]);
extern char* job_name(char *[], int );
extern void remove_job_by_pid(pid_t , jobs *[]);
extern void remove_by_pid(char *, int );
extern void remove_by_nb_of_job(char *, int );
extern void free_jobs(jobs *[]);
extern void counting_jobs(prompt*, jobs *[]);
extern void update_jobs(jobs *[], prompt*);
extern int etat_job(int , pid_t);


#endif