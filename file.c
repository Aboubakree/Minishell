#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include "minishell.h"



int main(int argc, char **argv, char **base_env)
{
    pid_t pid;

    pid = fork();
    if (pid == 0)
        exit(0);
    printf("parent : %d\n", (int)getpid());
    printf("pid : %d \n", (int)pid);
}