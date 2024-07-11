#include "globals.h"
#include "prompt.h"
#include <stdlib.h>

int my_bg(int *accept, char *copligne, prompt *pt){
    accept=0;
    char *tok = strtok(copligne, " ");
    tok = strtok(NULL, " ");
    if(tok == NULL){
        fprintf(stderr, "Veuillez ajouter un argument\n");
    } else {
        if (tok[0]=='%'){
            int n = atoi(tok + 1);
            if (n > 0 && n < 100) {
                if (tab_jobs[n - 1] != NULL) {
                    kill(-(tab_jobs[n - 1]->pid), SIGCONT);
                    tab_jobs[n - 1]->status = "Running";
                    fprintf(stderr, "[%d]   %d        %s %s\n", tab_jobs[n - 1]->num_job, tab_jobs[n - 1]->id_group_proc, tab_jobs[n - 1]->status, tab_jobs[n - 1]->name);
                } else {
                    fprintf(stderr, "Le job n'existe pas\n");
                }
            } else {
                fprintf(stderr, "Le job n'existe pas\n");
            }
        }
    }
    free(copligne);
    update_jobs(tab_jobs, pt);
    return 0;
}