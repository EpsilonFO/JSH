#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include "jsh.h"

void free_tab(char **tab){
    for(int i = 0; tab[i] != NULL; ++i){
        if(tab[i]!=NULL) free(tab[i]);
    }
    free(tab);
}

char** extraire_commandes(const char* s1, int* nbSousChaines) {
    int taille = strlen(s1);
    *nbSousChaines = 0;

    // Première passe : Compter le nombre de sous-chaînes
    for (int i = 0; i < taille; i++) {
        if (s1[i] == '<' && i + 3 < taille && s1[i + 1] == '(') {
            int j = i + 3;
            while (j < taille && s1[j] != ')' && s1[j] != '\0') {
                j++;
            }
            if (s1[j] == ')' && j > i + 3) {
                (*nbSousChaines)++;
                i = j;
            }
        }
    }

    // Allocation de mémoire pour le tableau de sous-chaînes
    char** sousChaines = malloc((*nbSousChaines + 1) * sizeof(char*)); // +1 pour le pointeur NULL final
    if (sousChaines == NULL) {
        return NULL; // Gestion d'erreur d'allocation mémoire
    }

    int index = 0;
    for (int i = 0; i < taille; i++) {
        if (s1[i] == '<' && i + 3 < taille && s1[i + 1] == '(') {
            int j = i + 3;
            while (j < taille && s1[j] != ')' && s1[j] != '\0') {
                j++;
            }

            if (s1[j] == ')' && j > i + 3) {
                sousChaines[index] = malloc((j - i - 2) * sizeof(char));
                if (sousChaines[index] == NULL) {
                    // Gestion d'erreur d'allocation mémoire, libérer la mémoire déjà allouée
                    for (int k = 0; k < index; ++k) {
                        free(sousChaines[k]);
                    }
                    free(sousChaines);
                    return NULL;
                }
                strncpy(sousChaines[index], s1 + i + 3, j - i - 2);
                sousChaines[index][j - i - 3] = '\0';
                index++;
                i = j;
            }
        }
    }
    sousChaines[*nbSousChaines] = NULL; // Ajouter un pointeur NULL à la fin
    return sousChaines;
}

char *get_commande(char *arg){
    char *lignedup = strdup(arg);
    char *tok = strtok(lignedup, " ");
    if (tok != NULL) {
        char *result = (char *)malloc(strlen(tok) + 1);
        if (result != NULL) {
            strcpy(result, tok);
            free(lignedup);
            return result;
        }
        free(result);
    }
    free(lignedup);
    return tok;
}
void close_pipes_substit(int j, int pipetab[][2], int n){
    for (int i = 0; i < n; i++) {
        if (i != j) {
            close(pipetab[i][0]);
            close(pipetab[i][1]);
        }
    }
}
int substitution_exe(char *ligne, prompt *pt){
    int nb_commandes;
    int ret =0;
    char *commande  = get_commande(ligne);
    char **tab_commandes = extraire_commandes(ligne, &nb_commandes);
    pid_t pids[nb_commandes];
    pid_t pids2[nb_commandes];
    
    int pipetab[nb_commandes][2];
    for(int i = 0; i < nb_commandes; ++i){
        if(pipe(pipetab[i]) == -1){
            free(commande);
            free_tab(tab_commandes);
            exit(1);
        }
    }
    for(int i = 0; i<nb_commandes; ++i){
        pids[i] =fork();
        if(pids[i] == 0){
            close_pipes_substit(i, pipetab, nb_commandes);
            dup2(pipetab[i][1],STDOUT_FILENO);
            close(pipetab[i][1]);
            cmd(tab_commandes[i], pt, &ret);
            free(commande);
            free(tab_commandes);
            exit(1);
        }else{
            wait(NULL);
            pids2[i] =fork();
            if(pids2[i] == 0){
                close_pipes_substit(i, pipetab, nb_commandes);
                close(pipetab[i][1]);
                dup2(pipetab[i][0],STDIN_FILENO);
                close(pipetab[i][0]);
                cmd(commande, pt, &ret);
                free(commande);
                free_tab(tab_commandes);
                exit(1);
            }
        }
    }
    free(commande);
    free_tab(tab_commandes);
    return ret;
}


