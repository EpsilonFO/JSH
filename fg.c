#include <stdlib.h>
#include <unistd.h>
#include "prompt.h"
#include "globals.h"
#include "jobs.h"

int my_fg(int *accept, char *copligne, prompt *pt){
    accept=0;
    char *tok = strtok(copligne, " ");
    tok = strtok(NULL, " ");
    if(tok==NULL){
        fprintf(stderr, "Veuillez ajouter un argument\n");
        free(copligne);
    }else{
        if (tok[0]=='%'){
            int n = atoi(tok+1);
            if (n>0 && n<100) {
                if (tab_jobs[n-1]!=NULL) {
                    sigset_t mask, oldmask;
                    sigemptyset(&mask);
                    sigaddset(&mask, SIGTTOU);
                    kill(-(tab_jobs[n-1]->pid), SIGCONT);
                    if (sigprocmask(SIG_BLOCK, &mask, &oldmask) < 0) {
                        perror("Erreur lors du blocage des signaux");
                        free(copligne);
                        exit(EXIT_FAILURE);
                    }
                    tcsetpgrp(STDIN_FILENO, tab_jobs[n-1] -> pid);
                    int k = etat_job(n-1, tab_jobs[n-1] -> pid);

                    if (k==1) {
                        remove_job_by_pid(tab_jobs[n-1] -> pid, tab_jobs);
                    }
                    if (tab_jobs[n-1]!=NULL) fprintf(stderr, "[%d]   %d        %s %s\n", tab_jobs[n-1] -> num_job, tab_jobs[n-1] -> id_group_proc, tab_jobs[n-1] -> status, tab_jobs[n-1] -> name);

                    tcsetpgrp(STDIN_FILENO, getpgrp());
                    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
                        perror("Erreur lors de la remise en place des signaux");
                        free(copligne);
                        exit(EXIT_FAILURE);
                    }
                }else{
                    fprintf(stderr, "Le job n'existe pas\n");
                }
            }else{
                fprintf(stderr, "Le job n'existe pas\n");
            }
        }
    }
    update_jobs(tab_jobs, pt);
    return 0;
}