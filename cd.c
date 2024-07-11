#include <stdlib.h>
#include <unistd.h>
#include "globals.h"
#include "prompt.h"
#include "cmd.h"

char *cd(char *path_courant, char *path_new, prompt *pt) {
    if (strcmp(path_new, "~")==0) {
        char *home = strdup(getenv("HOME"));
        if (chdir(home) == -1) {
            fprintf(stderr, "%s : No such file or directory\n", home);
            update_jobs(tab_jobs, pt);
            return NULL;
        }
        setenv("OLDPWD", path_courant, 1);
        return home;
    } else if (path_new[0]=='/') {
        if (chdir(path_new) == -1) {
            fprintf(stderr, "%s : No such file or directory\n", path_new);
            update_jobs(tab_jobs, pt);
            return NULL;
        }
        setenv("OLDPWD", path_courant, 1);
        char *new = strdup(path_new);
        update_jobs(tab_jobs, pt);
        return new;
    } else {
        char *path = malloc(sizeof(char)*(strlen(path_courant)+strlen(path_new)+2));
        sprintf(path, "%s/%s", path_courant, path_new);
        size_t path_size = pathconf(".", _PC_PATH_MAX);
        if (strcmp(path_new, "-")==0) {
            if (chdir(getenv("OLDPWD")) == -1) {
                fprintf(stderr, "%s : No such file or directory\n", getenv("OLDPWD"));
                free(path);
                update_jobs(tab_jobs, pt);
                return NULL;
            }
        } else {
            if (chdir(path) == -1) {
                fprintf(stderr, "%s : No such file or directory\n", path_new);
                free(path);
                update_jobs(tab_jobs, pt);
                return NULL;
            }
        }
        char *cwd = malloc(sizeof(char)*(path_size + 1));
        if (getcwd(cwd, path_size) == NULL) {
            free(cwd);
            free(path);
            update_jobs(tab_jobs, pt);
            return NULL;
        } 
        setenv("OLDPWD", path_courant, 1);
        free(path);
        update_jobs(tab_jobs, pt);
        return cwd;
    }
    update_jobs(tab_jobs, pt);
    return NULL;
}

int cd_context(char *copligne, int *return_value, prompt *pt){
    char *tok = strtok(copligne, " ");
    tok = strtok(NULL, " ");
    if (tok!=NULL) {
        char *path = strdup(tok);
        tok = strtok(NULL, " ");
        if (tok == NULL) {
            char *w = cd(pt->path, path, pt);
            if (w != NULL) {
                free(pt -> path);
                pt->path = strdup(w);
                free(w);
                free(copligne);
            } else {
                *return_value = 1;
            }
        } else {
            printf("cd : trop d'arguments\n");
            free(copligne);
        }
        free(path);
    } else {
        char *w = cd(pt->path, "~", pt);
        if (w!=NULL) {
            free(pt->path);
            pt->path = strdup(w);
            free(w);
        }else *return_value = 1;
        free(copligne);
    }
    update_jobs(tab_jobs, pt);
    return 0;
}