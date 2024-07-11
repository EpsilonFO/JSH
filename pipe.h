#ifndef PIPE_H
#define PIPE_H

#include <sys/types.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "prompt.h"

extern int nb_pipes(char*);
extern int pipe_exe(char*, int, prompt*);
extern void close_pipe(int, int[][2], int);
extern int cmd(char*, prompt*,int*);

#endif