#include "jobs.h"
#include "prompt.h"
#include "globals.h"
#include <stdlib.h>

int my_kill(int *accept, char *copligne, prompt *pt){
    accept=0;
    char *tok = strtok(copligne, " ");
    tok = strtok(NULL, " ");
    if(tok==NULL){
        fprintf(stderr, "Veuillez ajouter un argument\n");
    }else{
        if(tok[0]=='%'){ // kill %jobs
            remove_by_nb_of_job(tok+1, SIGTERM);

        }else if (tok[0]=='-'){
            char * stockage=strdup(tok);// à libérer
            tok=strtok(NULL, " ");
            if(tok == NULL){
                fprintf(stderr, "Mauvaise syntaxe\n"); // cas kill -12 sans mentionner de pid ou %jobs
                free(copligne);
                return 0;
            }else{
                int n= atoi(stockage+1);
                if(tok[0]=='%'){ // kill -signal %jobs 
                    
                        remove_by_nb_of_job(tok+1,n );
                }else{ // on a un kill -signal pid 
                    remove_by_pid(tok+1, n);
                }
            }free(stockage);
        }else{ //kill pid
            
            remove_by_pid(tok+1, SIGTERM);
        }
        
    }
    accept=1;
    free(copligne);
    update_jobs(tab_jobs, pt);
    return 0;
}