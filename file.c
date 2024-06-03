#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <string.h>
#include <signal.h>
#include <errno.h>


int main(int argc, char **argv, char **base_env)
{
    if (unlink("file_notexist"))
        perror("unlink");
    // printf("base : %p\nbase[0] : %s\n", base_env, base_env[0]);
}