/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 09:11:10 by akrid             #+#    #+#             */
/*   Updated: 2024/06/04 14:11:07 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void swap_data(t_environment *helper)
{
    char *key_temp;
    char *value_temp;

    key_temp = helper->key;
    helper->key = helper->next->key;
    helper->next->key = key_temp;
    value_temp = helper->value;
    helper->value = helper->next->value;
    helper->next->value = value_temp;
}

void sort(t_environment **sort_env)
{
    int b;
    t_environment *helper;

    b = 1;
    while (b)
    {
        b = 0;
        helper = *sort_env;
        while (helper && helper->next )
        {
            if (strncmp(helper->key, helper->next->key, ft_strlen(helper->key) + 1) > 0)
            {
                swap_data(helper);
                b = 1;
            }
            helper = helper->next;
        }
    }
}

t_environment *duplicate_env(t_environment *env)
{
    t_environment *copy;

    copy = NULL;
    while (env)
    {
        if (env->value == NULL)
            env_add_back(&copy, env_node(ft_strdup(env->key), NULL));
        else
            env_add_back(&copy, env_node(ft_strdup(env->key), ft_strdup(env->value)));
        env = env->next;
    }
    return (copy);
}

void free_copy(t_environment *env)
{
    t_environment *temp;

    temp = env;
    while (env)
    {
        temp = env;
        env = env->next;
        free(temp->key);
        free(temp->value);
        free(temp);
    }
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
        if (strncmp(helper->key, "_", 2) != 0 && ft_strncmp(helper->key, "?", 2) != 0)
        {
            if (helper->value != NULL)
                printf("declare -x %s=\"%s\"\n",helper->key, helper->value);
            else
                printf("declare -x %s\n",helper->key);
        }
        helper = helper->next;
    }
    free_copy(copy_env);
    return (0);
}

t_environment *extract_node(char *arg)
{
    t_environment *new;
    char *equal;
    char *key;
    char *value;

    equal = ft_strchr(arg, '=');
    if (equal == NULL)
        new = env_node(ft_strdup(arg), NULL);
    else
    {
        key = ft_substr(arg, 0, equal - arg);
        value = ft_substr(equal + 1, 0, ft_strlen(arg));
        new = env_node(key, value);
    }
    return (new);
}

void parse_error(char *arg)
{
    write(2, "bash: export: ", ft_strlen("bash: export: "));
    write(2, arg, ft_strlen(arg));
    write(2, ": not a valid identifier\n", ft_strlen(": not a valid identifier\n"));
}

int pars_node(t_environment *new, char *arg)
{
    int i;
    int plus;
    
    i = 0;
    plus = 0;
    while(new && new->key && new->key[i])
    {
        if ((new->key[i] < 'a' || new->key[i] > 'z') 
            && (new->key[i] < 'A' || new->key[i] > 'Z')
            && (new->key[i] < '0' || new->key[i] > '9')
            && new->key[i] != '_' && new->key[i] != '+')
                return (parse_error(arg),1);
        if (new->key[i] == '+')
            plus ++;
        if (plus > 1 || (new->key[0] >= '0' && new->key[0] <= '9'))
            return (parse_error(arg),1);
        i ++;
    }
    if (new->key[ft_strlen(new->key) - 1] == '+' && new->value == NULL)
        return (parse_error(arg),1);
    return (0);
}

void append_value(t_environment *env, t_environment *new)
{
    t_environment *target;
    t_environment *temp;
    t_environment *previous;
    char *helper;

    new->key[ft_strlen(new->key) - 1] = '\0';
    target = env_get_bykey(env, new->key);
    temp = env->next->next;
    previous = env->next;
    if (target == NULL)
    {
        previous->next = new;
        new->next = temp;
    }
    else if (new->value != NULL)
    {
        helper = ft_strjoin(target->value, new->value);
        free(target->value);
        target->value = helper;
        free_environment(new);
    }
    else
        free_environment(new);
}

void replace_value(t_environment *env, t_environment *new)
{
    t_environment *target;
    t_environment *temp;
    t_environment *previous;

    target = env_get_bykey(env, new->key);
    temp = env;
    previous = (env)->next;
    if (target == NULL)
    {
        temp = temp->next->next;
        previous->next = new;
        new->next = temp;
    }
    else if (new->value != NULL)
    {
        free(target->value);
        target->value = ft_strdup(new->value);
        free_environment(new);
    }
    else
        free_environment(new);
}

void add_node(t_environment *env, t_environment *new)
{
    if (strchr(new->key, '+'))
        append_value(env, new);
    else
        replace_value(env, new);
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
        exit(exit_value);
}


void export(t_minishell *minishell, t_environment **env)
{
    int exit_value;
    
    open_files(minishell);
    handel_input_output(minishell);
    if (args_count(minishell->args) < 2)
        exit_value = export_print(env);
    else
        exit_value = export_add(minishell, *env);
    handel_exit_status(*env, minishell->nbr_cmd, exit_value);
}
