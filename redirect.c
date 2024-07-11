
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "jsh.h"

int gerer_redirection(char *signe, char **fichier_entree, char **fichier_sortie, char **fichier_sortie_err, int redirection_mode[]) {
    char *tok = strtok(NULL, " ");
    if (tok != NULL) {
        if(strcmp(signe, "<") == 0){
            *fichier_entree = strdup(tok);
            return 0;
        }else{
            if (strcmp(signe, ">") == 0 ) {
                *fichier_sortie = strdup(tok);
                redirection_mode[0] = 1;
                return 0;
            }else if(strcmp(signe, ">|") == 0){
                *fichier_sortie = strdup(tok);
                redirection_mode[0] = 2;
                return 0;
            }else if(strcmp(signe, ">>") == 0){
                *fichier_sortie = strdup(tok);
                redirection_mode[0] = 3;
                return 0;
            }else if (strcmp(signe, "2>") == 0 ) {
                *fichier_sortie_err = strdup(tok);
                redirection_mode[1] = 4;
                return 0;
            }else if(strcmp(signe, "2>|") == 0){
                *fichier_sortie_err = strdup(tok);
                redirection_mode[1] = 5;
                return 0;
            }else if(strcmp(signe, "2>>") == 0){
                *fichier_sortie_err = strdup(tok);
                redirection_mode[1] = 6;
                return 0;
            }
        }
    }else {
        fprintf(stderr, "%s", "Erreur de syntaxe\n");
        return 1;
    }
    return 0;
}


void gerer_redirection_entree(char *fichier_entree) {
    int fd = open(fichier_entree, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr,"Erreur lors de l'ouverture du fichier de redirection d'entrée");
        exit(EXIT_FAILURE);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
}

void gerer_redirection_sortie(char *fichier_sortie, int redirection_mode) {
    int fd = 0;
    switch(redirection_mode){
        case 1 :
        fd = open(fichier_sortie, O_WRONLY | O_CREAT | O_EXCL, 0644);
        break;
        case 3 : 
        fd = open(fichier_sortie, O_WRONLY | O_CREAT | O_APPEND, 0644);
        break;
        case 2 : 
        fd = open(fichier_sortie, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        break;
        case 4 :
        fd = open(fichier_sortie, O_WRONLY | O_CREAT | O_EXCL, 0644);
        break;
        case 6 : 
        fd = open(fichier_sortie, O_WRONLY | O_CREAT | O_APPEND, 0644);
        break;
        case 5 : 
        fd = open(fichier_sortie, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        break;
        default:
        break;
    }
    if (fd == -1) {
        fprintf(stderr,"Erreur lors de l'ouverture du fichier de redirection de sortie\n");
        exit(EXIT_FAILURE);
    }
    if(redirection_mode >3){
        dup2(fd, STDERR_FILENO);
    }else{
        dup2(fd, STDOUT_FILENO);
    }
    close(fd);
}

