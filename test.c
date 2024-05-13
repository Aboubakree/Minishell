#include "minishell.h"

void exe(int exit_stat, int sl)
{
    printf("hello from child %d\n", exit_stat);
    sleep(sl);
    exit(exit_stat);
}


int main()
{
    pid_t pid[5];
    int status = -1;
    int i = 0;
    pid_t p;

    while (i < 5)
    {
        pid[i] = fork();
        if (pid[i] == 0)
            exe(i, i*2);
        i ++;
    }
    i = 0;
    while (i < 5)
    {
        p = 0;
        p = wait(&status);
        if (p)
        {
            status = status >> 8;
            printf("process %d exited with ststus : %d\n", (int)p, status);
            i ++;
        }
    }
    printf("hello from parent\n");
}