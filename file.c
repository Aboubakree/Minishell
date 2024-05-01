#include "minishell.h"



int main()
{
    char *t[] = {"cat" , "-e",  "Makefile" , ">" ,"tt", NULL};

    execve("/bin/cat", t, NULL);
    printf("error : %s\n", strerror(errno));
}