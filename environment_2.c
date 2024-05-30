/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 11:34:29 by akrid             #+#    #+#             */
/*   Updated: 2024/05/30 15:59:23 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int env_size(t_environment *env)
{
    int i;
    t_environment *temp;

    i = 0;
    temp = env;
    while (temp)
    {
        temp = temp->next;
        i ++;
    }
    return (i);
}

char **convert_env(t_environment *env)
{
    char **env_convert;
    char *temp;
    int i;

    i = 0;
    env_convert = malloc(sizeof(char *) * env_size(env) + 1);
    while (env)
    {
        temp = ft_strjoin(env->key, "=");
        env_convert[i] = ft_strjoin(temp, env->value);
        free(temp);
        i ++;
        env = env->next;
    }
    env_convert[i] = NULL;
    return (env_convert);
}
