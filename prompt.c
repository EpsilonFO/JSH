#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jsh.h"

char *colors(int n){
    char *color = malloc(sizeof(char)*18);
    if(color == NULL){
        fprintf(stderr, "Erreur malloc\n");
        exit(EXIT_FAILURE);
    }
    sprintf(color, "\001\033[%dm\002",n);
    color[17]='\0';
    return color;
}
int nb_of_digit(int n){
    int count = 0;
    if(n==0) return 1;
    while(n!=0){
        n=n/10;
        count++;
    }
    return count;
}
char *number_of_jobs(int n){
    int nj = 3+nb_of_digit(n);
    char *noj = malloc(sizeof(char)*nj);
    if (noj == NULL) {
        fprintf(stderr, "Erreur malloc\n");
        exit(EXIT_FAILURE);
    }
    sprintf(noj, "[%d]", n);
    noj[nj-1]='\0';
    return noj;
}
void prompt_final(char *final_prompt, char *color_jobs, char *jobs, char *color_path, char *path){
    sprintf(final_prompt,"%s%s%s%s%s", color_jobs, jobs, color_path, path, "\001\033[00m\002$ ");
}

prompt *new_prompt(int jobs, int clr_job, int clr_path, char *path){
    char *color_jobs = colors(clr_job);
    char *color_path = colors(clr_path);
    char *nb_of_jobs = number_of_jobs(jobs);
    char *path2 = malloc(sizeof(char)*(strlen(path)+1));
    strcpy(path2, path);
    char *final_prompt = malloc(sizeof(char)*(52+strlen(nb_of_jobs)+strlen(path2)));
    prompt_final(final_prompt, color_jobs, nb_of_jobs, color_path, path2);
    final_prompt[51+strlen(nb_of_jobs)+strlen(path2)]='\0';
    prompt *pt = malloc(sizeof(prompt));
    pt -> jobs = jobs;
    pt -> color_jobs = color_jobs;
    pt -> color_path = color_path;
    pt -> path = path2;
    pt -> truncated_path = malloc(sizeof(char)*30);
    pt -> final_prompt = final_prompt;
    free(nb_of_jobs);
    return pt;
}


void truncate_prompt(prompt *pt){
    char *nb_of_jobs = number_of_jobs(pt -> jobs);
    if(strlen(pt -> path)+strlen(nb_of_jobs)>25){
        char *truncated = malloc(sizeof(char)*26);
        truncated[0] = '.';truncated[1] = '.';truncated[2] = '.';
        for(size_t i=0;i<22;++i){
            truncated[i+3] = pt -> path[strlen(pt -> path)-22+i+strlen(nb_of_jobs)-3];
        }
        truncated[25]='\0';
        if (pt->truncated_path != NULL) {
            strcpy(pt->truncated_path, truncated);
        }
        prompt_final(pt -> final_prompt, pt -> color_jobs, nb_of_jobs, pt -> color_path, pt -> truncated_path);
        free(truncated);
    } else {
        prompt_final(pt -> final_prompt, pt -> color_jobs, nb_of_jobs, pt -> color_path, pt -> path);
    }
    free(nb_of_jobs);
}

void free_prompt(prompt *pt){
    free(pt -> color_jobs);
    free(pt -> color_path);
    free(pt -> path);
    free(pt -> truncated_path);
    free(pt -> final_prompt);
    free(pt);
}
