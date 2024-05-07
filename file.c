// #include "minishell.h"
#include "libft/libft.h"


int main()
{
    char **str = ft_split("   ", ' ');
    printf("%p\n", str);

    return (0);
}