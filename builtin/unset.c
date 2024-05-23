/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 09:09:02 by akrid             #+#    #+#             */
/*   Updated: 2024/05/23 12:24:17 by akrid            ###   ########.fr       */
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

    if (minishell->args && args_count(minishell->args) == 1)
        return;
    i = 1;
    while (minishell->args && minishell->args[i])
    {
        target = env_get_bykey(*env, minishell->args[i]);
        if (target == NULL)
        {
            i ++;
            continue;
        }
        else if (target == *env)
        {
            *env = (*env)->next;
            free(target);
        }
        else 
        {
            previous = get_before_target(*env, target);
            previous->next = target->next;
            free(target);
        }
        i ++;
    }
}
