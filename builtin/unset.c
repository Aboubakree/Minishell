/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 09:09:02 by akrid             #+#    #+#             */
/*   Updated: 2024/05/31 10:23:32 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_environment *get_before_target(t_environment *env, t_environment *target)
{
    t_environment *previous;

    previous = NULL;
    while (env)
    {
        if (env == target)
            return (previous);
        previous = env;
        env = env->next;
    }
    return (NULL);
}


void unset(t_minishell *minishell, t_environment **env)
{
    t_environment *target;
    t_environment *previous;
    int           i;

    open_files(minishell);
    handel_input_output(minishell);
    if (minishell->args && args_count(minishell->args) == 1)
        return;
    i = 1;
    while (minishell->args && minishell->args[i])
    {
        target = env_get_bykey(*env, minishell->args[i]);
        if (target == NULL || ft_strncmp(target->key, "_", 2) == 0)
        {
            i ++;
            continue;
        }
        else if (target == *env)
        {
            *env = (*env)->next;
            if (target->key)
                free(target->key);
            if (target->value)
                free(target->value);
            free(target);
        }
        else 
        {
            previous = get_before_target(*env, target);
            previous->next = target->next;
            if (target->key)
                free(target->key);
            if (target->value)
                free(target->value);
            free(target);
        }
        i ++;
    }
}
