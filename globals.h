#ifndef GLOBALS_H
#define GLOBALS_H

#include <sys/types.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "jobs.h"

extern jobs *tab_jobs[100];
extern sig_atomic_t fini;
extern int accept;

#endif