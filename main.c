/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:18:34 by akrid             #+#    #+#             */
/*   Updated: 2024/05/01 12:29:15 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int main(int argc, char **argv, char **base_env)
{
    t_environment *env;
    char *equal;
    char **splt;
    
    (void)argv;
    env = NULL;
    splt = NULL;
    equal = NULL;
    if (argc > 1)
        return (1);
    get_environment(&env, base_env);
    char *str;
    while (1)
    {
        str = readline("minishell-->$");
        if (str == NULL)
            break;
        free(str);
    }
    return (0);
}
