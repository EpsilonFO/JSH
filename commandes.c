
#include <stdlib.h>
#include "jsh.h"
#include "globals.h"
#include "functions.h"
#include "cmd.h"

int cmd(char *ligne, prompt *pt, int *return_value){
    char *copligne = malloc(sizeof(char)*(strlen(ligne)+1));
    strcpy(copligne, ligne);
    if (estVideOuEspaces(ligne)) {
        free(copligne);
        return 0;
    }
    else if(strcmp(ligne, "?")==0){
        printf("%d\n", *return_value);
        *return_value = 0;
        free(copligne);
        return 0;
    }
    else if(strncmp(ligne, "exit", 4)==0) {
        return my_exit(pt, copligne, return_value);
    }
    else if(strcmp(ligne, "pwd")==0) {
        printf("%s\n", pt-> path);
        free(copligne);
    }
    else if(strncmp(ligne, "kill", 4)==0){
        return my_kill(&accept, copligne, pt);
    }
    else if (strncmp(ligne, "fg", 2)==0) {
        return my_fg(&accept, copligne, pt);
    }
    else if (strncmp(ligne, "bg", 2) == 0) {
        return my_bg(&accept, copligne, pt);
    }
    else if(strncmp(ligne, "cd", 2)==0) {
        return cd_context(copligne, return_value, pt);
        
    }else if(strncmp(ligne, "jobs", 4)==0){
        accept=0;
        function_traite_processus();
        for(size_t i=0;i<100;++i){
            if(tab_jobs[i]!=NULL){
                    fprintf(stdout, "[%d] %d  %s  %s\n", tab_jobs[i] -> num_job+1,tab_jobs[i]->pid, tab_jobs[i] -> status, tab_jobs[i] -> name);
                }
        }
        accept=1;
    }
    else {
        return manage(ligne, copligne, pt, return_value);
    }
    update_jobs(tab_jobs, pt);
    return 0;
}