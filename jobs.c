#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "jobs.h"
#include "globals.h"
#include "functions.h"

void add_job(jobs *j, jobs *tab_jobs[]){
    for(size_t i=0;i<100;++i){
        if(tab_jobs[i]==NULL){
            j -> num_job = i;
            tab_jobs[i]=j;
            break;
        }
    }
}

void remove_job(jobs *j, jobs *tab_jobs[], prompt *pt){
    free(j -> name);
    free(j);
    tab_jobs[j -> num_job]=NULL;
}

void list_jobs(jobs *tab_jobs[]){
    for(size_t i=0;i<100;++i){
        if(tab_jobs[i]!=NULL){
            fprintf(stderr, "[%d] %d  %s  %s\n", tab_jobs[i] -> num_job, tab_jobs[i] -> id_group_proc, tab_jobs[i] -> status, tab_jobs[i] -> name);
        }
    }
}

void remove_job_by_pid(pid_t pid, jobs *tab_jobs[]){
    for(size_t i=0;i<100;++i){
        if(tab_jobs[i]!=NULL){
            if(tab_jobs[i] -> pid == pid){
                free(tab_jobs[i] -> name);
                free(tab_jobs[i]);
                tab_jobs[i]=NULL;
                break;
            }
        }
    }
}

void free_jobs(jobs *tab_jobs[]){
    for(size_t i=0;i<100;++i){
        if(tab_jobs[i]!=NULL){
            if(tab_jobs[i] -> name!=NULL) free(tab_jobs[i] -> name);
            free(tab_jobs[i]);
        }
    }
    free(tab_jobs);
}
char *job_name(char *tab[], int compt){
    int size = 1;
    for(size_t i = 0; i < compt; ++i){
        size += strlen(tab[i]) + 1;
    }

    char *name = malloc(size * sizeof(char));
    if (name == NULL) return NULL;
    name[0] = '\0';

    for(size_t i = 0; i < compt; ++i){
        strcat(name, tab[i]);
        if (i < compt - 1) {
            strcat(name, " ");
        }
    }
    return name;
}

void remove_by_nb_of_job(char *n, int signal){
    int x = atoi(n);
    if(tab_jobs[x-1]!=NULL){

        if(kill(-getpgid((tab_jobs[x-1]->pid)),signal)==-1){
            perror("error kill");
            return;
        }
        
    }else fprintf(stderr, "Ce processus n'existe pas\n");
}
void remove_by_pid(char*n, int signal ){
    int x= atoi(n);
    for(size_t i=0; i<100 ;++i){
        if(tab_jobs[i]!=NULL){
            
        if(kill((tab_jobs[x-1]->pid),signal)==-1){
            perror("error kill");
            return;
        }
        
        }
    }
}

int etat_job(int num, pid_t pid) {
    int a=0;
    int b=0;
    int c=0;

    a=waitpid(pid, &b, WUNTRACED);
    do {
        if (WIFEXITED(b)) {
            tab_jobs[num] -> status = "Done";
            c=1;
        }
        if(WIFSIGNALED(b)){
            tab_jobs[num] -> status = "Killed";
            c=1;
        }
        if(WIFSTOPPED(b)){
            tab_jobs[num] -> status = "Stopped";
            c=0;
        } 
    } while (a == -1);
    return c;
}

void counting_jobs(prompt *pt, jobs **tab_jobs){
    pt -> jobs = 0;
    for (int i=0; i<100; ++i) {
        if (tab_jobs[i]!=NULL) {
            pt->jobs++;
        }
    }
}

void update_jobs(jobs **tab_jobs, prompt *pt){
    if (fini==1){
        function_traite_processus();
        fini=0;
        for (int i=0; i<100; ++i) {
            if (tab_jobs[i]!=NULL) {
                if (strcmp(tab_jobs[i] -> status, "Done") == 0 || strcmp(tab_jobs[i] -> status, "Killed") == 0) {
                    remove_job_by_pid(tab_jobs[i] -> pid, tab_jobs);
                }
            }
        }
    }
    counting_jobs(pt, tab_jobs);
}
