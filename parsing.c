#include "minishell.h"

int main(int argc, char **argv, char **base_env)
{
    t_environment *env;
    t_token *tokens;
    t_minishell *minishell;
    char *str;
    
    (void)argv;
    env = NULL;
    if (argc > 1)
        return (1);
    get_environment(&env, base_env);
    printf("Welcome to minishell\n");
    while (1)
    {
        str = readline("\033[0;32mminishell$ \033[0m");
        if (str == NULL)
            break;
        printf("%s\n", str);
        free(str);
    }
    free_environment(env);
    return (0);
}