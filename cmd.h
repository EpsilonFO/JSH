#ifndef CMD_H
#define CMD_H

#include <sys/types.h>
#include "jobs.h"
#include "prompt.h"

extern int cmd(char*, prompt*,int*);

extern int my_exit(prompt*,char *,int*);
extern int my_kill(int*, char *, prompt*);
extern int my_fg(int*, char *, prompt*);
extern int my_bg(int*, char *, prompt*);
extern int manage(char *, char *, prompt *, int *);
extern char *cd(char *, char*, prompt*);
extern int cd_context(char *, int *, prompt *);

#endif