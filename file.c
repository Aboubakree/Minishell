#include "minishell.h"

int main()
{
    int x;

    x = dup(STDIN_FILENO);
    printf("%d\n", x);
    int fd = open("test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    

    close(fd);
}
