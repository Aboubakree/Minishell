/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:18:34 by akrid             #+#    #+#             */
/*   Updated: 2024/04/27 13:46:45 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_envirement *env_node(char *key, char *value)
{
    t_envirement *new;
    
    new = malloc(sizeof(t_envirement));
    if (new == NULL)
        exit(1);
    new->key = key;
    new->value = value;
    new->next = NULL;
    return (new);
}

void    env_add_back(t_envirement **env, t_envirement *node)
{
    t_envirement *temp;
    
    temp = NULL;
    if (temp == NULL)
        *env = node;
    else
    {
        temp = *env;
        while (temp->next)
            temp = temp->next;
        temp->next = node;
    }
}

void get_envirement(t_envirement **env, char **base_env)
{
    int i;
    char *equal;
    char *key;
    char *value;

    *env = NULL;
    i = 0;
    while (base_env[i])
    {
        equal = ft_strchr(base_env[i], '=');
        key = ft_substr(base_env[i], 0, base_env[i] - equal);
        value = ft_substr(equal + 1, 0, ft_strlen(base_env[i]));
        env_add_back(env, env_node(key, value));
        i ++;
    }
}

int main(int argc, char **argv, char **e)
{
    t_envirement *env;
    
    (void)argv;
    if (argc > 1)
        return (1);
    get_envirement(&env, e);
    // char *str;
    // while (1)
    // {
    //     str = readline("minishell-->$");
    //     if (str == NULL)
    //         break;
    //     if (ft_strncmp("env", str, 10) == 0)
    //     {
    //         t_envirement *temp;
    //         temp = env;
    //         while (temp)
    //         {
    //             printf("%s=%s\n", temp->key, temp->value);
    //             temp = temp->next;
    //         }
    //     }
    //     else
    //         printf("Your line : %s\n", str);
    //     free(str);
    // }
    // return (0);
}
