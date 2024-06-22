/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 09:11:10 by akrid             #+#    #+#             */
/*   Updated: 2024/06/11 11:17:16 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void add_node(t_environment *env, t_environment *new)
{
    if (strchr(new->key, '+'))
        append_value(env, new);
    else
        replace_value(env, new);
}

int    export_print( t_environment **env)
{
    t_environment *copy_env;
    t_environment *helper;

    copy_env = duplicate_env(*env);
    sort(&copy_env);
    helper = copy_env;
    while (helper)
    {
        if (strncmp(helper->key, "_", 2) != 0 && ft_strncmp(helper->key, "?", 2) != 0 && ft_strncmp(helper->key, "PATH", 2) != 0)
            print(helper->key, helper->value);
        if (ft_strncmp(helper->key, "PATH", 2) == 0 && env_size(*env) > 10)
            print(helper->key, helper->value);
        helper = helper->next;
    }
    free_copy(copy_env);
    return (0);
}


int export_add(t_minishell *minishell, t_environment *env)
{
    t_environment *new;
    int i;
    int exit_value;

    i = 1;
    new = NULL;
    exit_value = 0;
    while (minishell->args && minishell->args[0] && minishell->args[i])
    {
        new = extract_node(minishell->args[i]);
        if (pars_node(new, minishell->args[i]))
        {
            free_environment(new);
            if (minishell->nbr_cmd == 1)
                exit_value = 1;
            i ++;
            continue;
        }
        add_node(env, new);
        i ++;
    }
    return (exit_value);
}

void handel_exit_status(t_environment *env, int nbr_cmd , int exit_value)
{
    if (nbr_cmd == 1)
        set_exit_status(env, exit_value);
    else
    {
        free_at_exit();
        exit(exit_value);
    }
}


void export(t_minishell *minishell, t_environment **env)
{
    int exit_value;
    
    if (open_files(minishell, *env, minishell->files))
        return ;
    handel_input_output(minishell);
    if (args_count(minishell->args) < 2)
        exit_value = export_print(env);
    else
        exit_value = export_add(minishell, *env);
    handel_exit_status(*env, minishell->nbr_cmd, exit_value);
}
