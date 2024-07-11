#ifndef JSH_H
#define JSH_H

#include <sys/types.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "jobs.h"
#include "prompt.h"

extern void function_traite_processus();
extern int jsh();
extern int commandes_externes(char * [], jobs * [], unsigned , int , char *,char *, char *, int []);
extern int cmd(char*, prompt*,int*);
extern int substitution_exe(char *, prompt*);

#endif

