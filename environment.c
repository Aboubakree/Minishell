/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 18:53:56 by akrid             #+#    #+#             */
/*   Updated: 2024/05/14 09:19:05 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_environment *env_node(char *key, char *value)
{
    t_environment *new;
    
    new = malloc(sizeof(t_environment));
    if (new == NULL)
        exit(1);
    new->key = key;
    new->value = value;
    new->next = NULL;
    return (new);
}

void    env_add_back(t_environment **env, t_environment *node)
{
    t_environment *temp;
    
    temp = NULL;
    if (*env == NULL)
        *env = node;
    else
    {
        temp = *env;
        while (temp->next)
            temp = temp->next;
        temp->next = node;
    }
}

void get_environment(t_environment **env, char **base_env)
{
    int i;
    char *equal;
    char *key;
    char *value;

    i = 0;
    while (base_env[i])
    {
        equal = ft_strchr(base_env[i], '=');
        key = ft_substr(base_env[i], 0, equal - base_env[i]);
        value = ft_substr(equal + 1, 0, ft_strlen(base_env[i]));
        env_add_back(env, env_node(key, value));
        i ++;
    }
}

t_environment *env_get_bykey(t_environment *env, char *key)
{
    t_environment *temp;

    temp = env;
    while (temp)
    {
        if (ft_strncmp(temp->key, key, ft_strlen(temp->key)) == 0)
            return (temp);
        temp = temp->next;
    }
    return (NULL);
}
