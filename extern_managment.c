#include <stdlib.h>
#include <unistd.h>
#include "prompt.h"
#include "globals.h"
#include "pipe.h"
#include "substitution.h"
#include "redirect.h"
#include "functions.h"

int char_length(char *ligne){
    int a=0;
    while(ligne[a]!='\0'){
        a++;
    }
    return a;
}

int test_if_pipe_or_substitute(char *ligne, char *copligne, int a, int *return_value, prompt *pt){
    for(size_t i=0;i<a;++i){
        if(copligne[i]=='|' && i>0 && copligne[i-1]==' '){
            *return_value = pipe_exe(ligne, nb_pipes(ligne), pt);
            free(copligne);
            return 1;
        }
        if(copligne[i]=='<' && i>0 && copligne[i+1]=='('){
            *return_value = substitution_exe(ligne, pt);
            free(copligne);
            return 1;
        }
    }
    return 0;
}

int manage(char *ligne, char *copligne, prompt *pt, int *return_value){
    char **tab;
    int compt = 0;
    int a= char_length(ligne);
    if(test_if_pipe_or_substitute(ligne, copligne, a, return_value, pt)) return 0;
    char *tok = strtok(copligne, " ");
    while (tok != NULL) {
        if (is_redirection(tok) && compt==0) return 1;
        else if(is_redirection(tok)) break;
        compt++;
        tok = strtok(NULL, " ");
    }

    tab = malloc (sizeof(char*)*(compt+1));
    if (tab == NULL) {
        fprintf(stderr,"%s","Erreur de malloc\n");
        free(tok);
        free(copligne);
        return 1;
    }
    char *ligne_dup = strdup(ligne);
    tok = strtok(ligne_dup, " ");
    int j = 0;
    char *fichier_entree = NULL;
    char *fichier_sortie = NULL;
    char *fichier_sortie_err = NULL;
    int redirection_mode[2];
    redirection_mode[0] = 0;
    redirection_mode[1] = 0;
    while (tok != NULL) {
        if (is_redirection(tok)) {
            if(gerer_redirection(tok, &fichier_entree, &fichier_sortie,&fichier_sortie_err, redirection_mode)) return 0;
            tok = strtok(NULL, " ");
        } else {
            tab[j] = tok;
            tok = strtok(NULL, " ");
            j++;
        }
    }
    unsigned b = 0;
    tab[compt] = NULL;
    if(strcmp(tab[compt -1],"&") == 0) {
        tab[compt -1] = NULL;
        pt -> jobs ++;
        compt --;
        b = 1;
    }
    else if(tab[compt -1][strlen(tab[compt-1])-1]=='&'){
        b = 1;
        tab[compt -1][strlen(tab[compt-1])-1]= '\0';
        pt -> jobs ++;
        compt --;
    }
    *return_value = commandes_externes(tab, tab_jobs, b, compt, fichier_entree, fichier_sortie, fichier_sortie_err, redirection_mode);
    free(copligne);
    free(ligne_dup);
    if(fichier_entree!=NULL) free(fichier_entree);
    if(fichier_sortie!=NULL) free(fichier_sortie);
    if(fichier_sortie_err!=NULL) free(fichier_sortie_err);
    update_jobs(tab_jobs, pt);
    return 0;
}