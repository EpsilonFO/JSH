#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "jobs.h"
#include "globals.h"
#include "redirect.h"
#include "functions.h"

sig_atomic_t fini=0;

void sigchld_handler(int sig) {
    fini=1;
}

void execute_status(char *stat, int pid){
    for (int i = 0; i < 100; ++i) {
        if (tab_jobs[i] != NULL) {
            if (tab_jobs[i] -> pid == pid) {
                tab_jobs[i] -> status = stat;
                if(accept) fprintf(stderr,"[%d]   %d        %s %s\n", tab_jobs[i] -> num_job, tab_jobs[i] -> id_group_proc, tab_jobs[i] -> status, tab_jobs[i] -> name);
                break;
            }
        }
    }
}
void function_traite_processus(){
    int status=0;
    pid_t pid=-1;
    while ((pid = waitpid(-1, &status, WNOHANG | WCONTINUED | WUNTRACED )) > 0) {
        if (WIFEXITED(status)) execute_status("Done", pid);
        if(WIFSIGNALED(status)) execute_status("Killed", pid);
        if(WIFSTOPPED(status)) execute_status("Stopped", pid);
        if(WIFCONTINUED(status)) execute_status("Running", pid);
    }
}

int commandes_externes(char *tab[], jobs *tab_jobs[], unsigned b, int compt,char *fichier_entree, char *fichier_sortie, char * fichier_sortie_err, int redirection_mode[]) {
    if(b){
        struct sigaction sa;
        sa.sa_handler = sigchld_handler;
        sigemptyset(&sa.sa_mask); 
        sa.sa_flags = SA_RESTART;
        sigaction(SIGCHLD, &sa, NULL);
    }
    pid_t f = fork();
    if (f == 0) {
        setpgid(getpid(),getpid());
         struct sigaction sa;
    sa.sa_handler = SIG_DFL; // Ignorer le signal en utilisant SIG_IGN
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGTTIN, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
    sigaction(SIGTTOU, &sa, NULL);
    sigaction(SIGTSTP, &sa, NULL);

        if (fichier_entree != NULL) gerer_redirection_entree(fichier_entree);
        if (fichier_sortie != NULL) gerer_redirection_sortie(fichier_sortie, redirection_mode[0]);
        if (fichier_sortie_err != NULL) gerer_redirection_sortie(fichier_sortie_err, redirection_mode[1]);
        
        execvp(tab[0], tab);
        fprintf(stderr, "%s : commande introouvable\n", tab[0]);
        exit(EXIT_FAILURE);
    } else if (f==-1) {
        fprintf(stderr, "%s : commande introoouvable\n", tab[0]);
        exit(EXIT_FAILURE);
    }
    else {
        int status=0;
        if(b){
            jobs *j = malloc(sizeof(jobs));
            char *job_n = job_name(tab, compt);
            j -> name = job_n;
            j -> pid = f;
            j -> status = "Running";
            j -> id_group_proc = 0;
            add_job(j, tab_jobs);
            fprintf(stderr,"[%d]   %d        %s %s\n", j -> num_job, j -> id_group_proc, j -> status, j -> name);
        }else{
            sigset_t mask, oldmask;
            sigemptyset(&mask);
            sigaddset(&mask, SIGTTOU);
            if (sigprocmask(SIG_BLOCK, &mask, &oldmask) < 0) {
                perror("Erreur lors du blocage des signaux");
                exit(EXIT_FAILURE);
            }
                        
            setpgid(f, f);
            tcsetpgrp(STDIN_FILENO,f);
            waitpid(f, &status, WUNTRACED);
            tcsetpgrp(STDIN_FILENO,getpgrp());
            if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
                perror("Erreur lors de la remise en place des signaux");
                exit(EXIT_FAILURE);
            }
        }
         
        if (WIFEXITED(status)) {
            free(tab);
            return WEXITSTATUS(status);
        }
        if(WIFSIGNALED(status)){
            free(tab);
            return EXIT_FAILURE;
        }
        if(WIFSTOPPED(status)){
            struct sigaction sa;
            sa.sa_handler = sigchld_handler;
            sa.sa_flags = SA_RESTART;
            sigaction(SIGCHLD, &sa, NULL);
            jobs *j = malloc(sizeof(jobs));
            char *job_n = job_name(tab, compt);
            j -> name = job_n;
            j -> pid = f;
            j -> status = "Stopped";
          
            j -> id_group_proc = 0;
            add_job(j, tab_jobs);
            
            fprintf(stderr,"[%d]   %d        %s %s\n",  j-> num_job, j-> id_group_proc, j-> status,j-> name);
            free(tab);   
        }
    }
    return EXIT_SUCCESS;
}


int estVideOuEspaces(const char *chaine) {
    while (*chaine) {
        if (!isspace(*chaine)) return 0;
        chaine++;
    }
    return 1; 
}

int is_redirection(char *tok){
    return (strcmp(tok, "<") == 0 || strcmp(tok, ">") == 0 || strcmp(tok, ">|") == 0 || strcmp(tok, ">>") == 0 || strcmp(tok, "2>") == 0 || strcmp(tok, "2>>") == 0 || strcmp(tok, "2>|") == 0);
}

