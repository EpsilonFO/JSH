#include <stdlib.h>
#include <unistd.h>
#include "jsh.h"

jobs *tab_jobs[100];
int accept=1;
int jsh (){
    struct sigaction sa;
    sa.sa_handler = SIG_IGN; // Ignorer le signal en utilisant SIG_IGN
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    // Ignorer les signaux spécifiés avec sigaction
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGTTIN, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTTOU, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);
    int return_value = 0;
    size_t path_size = pathconf(".", _PC_PATH_MAX);
    rl_outstream = stderr;
    if(tab_jobs==NULL){
        printf("Erreur de tab_jobs\n");
        return 1;
    }
    for(size_t i=0;i<100;++i) {
        tab_jobs[i] = NULL;
    }
    char *cwd = malloc(sizeof(char)*(path_size + 1));
    if (getcwd(cwd, path_size) == NULL) {
        printf("Erreur de getcwd\n");
        free(cwd);
        return 1;
    } 
    prompt *pt = new_prompt(0,32,34, cwd);
    free(cwd);
    char *ligne;
    while(1){
        truncate_prompt(pt);
        
        ligne = readline(pt -> final_prompt);
        if (ligne==NULL) {
            break;
        }
        add_history(ligne);
        if(cmd(ligne, pt, &return_value)) {
            free(ligne);
            break;
        }
        free(ligne);
    }
    free_prompt(pt);
    clear_history();
    return return_value;
}
