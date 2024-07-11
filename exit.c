#include <stdlib.h>
#include "cmd.h"
#include "globals.h"
#include "prompt.h"
#include "redirect.h"

int my_exit(prompt *pt, char *copligne, int *return_value){
    if(pt -> jobs!=0){
        fprintf(stderr, "Attention : il reste des jobs en cours d'exécution\n");
        *return_value=1;
        free(copligne);
        update_jobs(tab_jobs, pt);
        return 0;
    }
    char *tok = strtok(copligne, " ");
    tok = strtok(NULL, " ");
    if(tok!=NULL){
        if(is_redirection(tok)) {
            free(copligne);
            update_jobs(tab_jobs, pt);
            return 1;
        }
        *return_value = atoi(tok);
        tok = strtok(NULL, " ");
        if (tok == NULL) {
            free(copligne);
            update_jobs(tab_jobs, pt);
            return 1;
        }else {
            printf("exit : trop d'arguments\n");
            free(copligne);
            update_jobs(tab_jobs, pt);
            return 0;
        }
    }else{
        free(copligne);
        update_jobs(tab_jobs, pt);
        return 1;
    }
}