/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 09:09:02 by akrid             #+#    #+#             */
/*   Updated: 2024/06/04 14:29:03 by akrid            ###   ########.fr       */
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

void free_target(t_environment *target)
{
    if (target->key)
        free(target->key);
    if (target->value)
        free(target->value);
    free(target);
}

int check_valid_key(t_environment *target)
{
    if (target == NULL 
        || ft_strncmp(target->key, "_", 2) == 0 
        || ft_strncmp(target->key, "?", 2) == 0)
        return (1);
    return (0);
}

void unset(t_minishell *minishell, t_environment **env)
{
    t_environment *target;
    t_environment *previous;
    int           i;

    open_files(minishell);
    handel_input_output(minishell);
    i = 1;
    while (minishell->args && minishell->args[i])
    {
        target = env_get_bykey(*env, minishell->args[i ++]);
        if (check_valid_key(target))
            continue;
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
    }
    handel_exit_status(*env, minishell->nbr_cmd, 0);
}
