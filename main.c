/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:18:34 by akrid             #+#    #+#             */
/*   Updated: 2024/04/28 17:38:03 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"



int main(int argc, char **argv, char **base_env)
{
    t_environment *env;
    t_environment *OLDPWD;
    t_environment *PWD;
    // char            new_path[1024];
    
    (void)argv;
    env = NULL;
    if (argc > 1)
        return (1);
    get_environment(&env, base_env);
    char *str;
    while (1)
    {
        str = readline("minishell-->$");
        if (str == NULL)
            break;
        if (ft_strncmp("cd", str, 2) == 0)
        {
            cd(str + 3, env);
            OLDPWD = env_get_bykey(env, "OLDPWD");
            PWD = env_get_bykey(env, "PWD");
            // printf("%s=%s\n", OLDPWD->key, OLDPWD->value);
            // printf("%s=%s\n", PWD->key, PWD->value);
        }
        else if (ft_strncmp("pwd", str, 3) == 0)
            pwd(env);
        else
            printf("Your line : %s\n", str);
        free(str);
    }
    return (0);
}
