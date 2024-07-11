#ifndef PROMPT_H
#define PROMPT_H

#include <sys/types.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct prompt{
    int jobs;
    char *color_jobs;
    char *color_path;
    char *path;
    char *truncated_path;
    char *final_prompt;
} prompt;

extern char *colors(int);
extern int nb_of_digit(int);
extern char *number_of_jobs(int);
extern prompt *new_prompt();
extern void truncate_prompt(prompt *);
extern void free_prompt(prompt *);
void prompt_final(char *, char *, char *, char *, char *);

#endif