#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <sys/types.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "jobs.h"

extern sig_atomic_t fini;
extern void function_traite_processus();
extern int commandes_externes(char * [], jobs * [], unsigned , int , char *,char *, char *, int []);
extern int estVideOuEspaces(const char *);

#endif