#ifndef REDIRECT_H
#define REDIRECT_H

#include <sys/types.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

extern int gerer_redirection(char *, char **, char **, char **, int *);
extern void gerer_redirection_entree(char *);
extern void gerer_redirection_sortie(char *, int );
extern int is_redirection(char*);


#endif