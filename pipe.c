#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "jsh.h"

int nb_pipes(char *ligne){
    int n = 0;
    for(int i = 0; i< strlen(ligne); ++i){
        if(ligne[i] == '|') n++;
    }
    return n;
}
void close_pipe(int j, int pipetab[][2], int n){
    for(size_t i=0;i<n;++i){
        if(i != j) {
            close(pipetab[i][1]);
        }
        if(i != j-1) {
            close(pipetab[i][0]);
        }
    }
}
int only_spaces(char *ligne){
    for(int i = 0; i< strlen(ligne); ++i){
        if(ligne[i] != ' ') return 0;
    }
    return 1;
}

int pipe_exe(char *ligne, int nb_pipe, prompt *pt){
    if(only_spaces(ligne)) {
        fprintf(stderr, "%s", "Erreur de syntaxe\n");
        return 0;
    }
   char **tab = malloc(sizeof(char*) * (nb_pipe + 2));
   pid_t pids[nb_pipe+1];
    if (tab == NULL) {
        fprintf(stderr, "%s", "Erreur de malloc\n");
        return -1;
    }
    
    int pipetab[nb_pipe][2];
    for(int i = 0; i < nb_pipe; ++i){
        if(pipe(pipetab[i]) == -1){
            exit(1);
        }
    }
    
    int k = 0;
    char *copligne = strdup(ligne);
    char *tok = strtok(copligne, "|");
    int ret = 0;
    while (tok != NULL) {
        tab[k] = tok;
        pids[k] = fork();
        if(pids[k] == 0){
            close_pipe(k, pipetab, nb_pipe);
            if(k == 0){
                dup2(pipetab[k][1], STDOUT_FILENO);
            } else if(k == nb_pipe){
                dup2(pipetab[k-1][0], STDIN_FILENO);
            } else {
                dup2(pipetab[k-1][0], STDIN_FILENO);
                dup2(pipetab[k][1], STDOUT_FILENO);
            }
            cmd(tab[k],pt, &ret);
            exit(1);
        } else {
            if(k != 0){
                close(pipetab[k-1][0]);
            }
            if(k != nb_pipe){
                close(pipetab[k][1]);
            }
            k++;
            tok = strtok(NULL, "|");
        }
    }
    int status;
    for(int i = 0; i<k;++i ){
        wait(&status);
    }

    free(copligne);
    free(tab);
    return ret;
}
