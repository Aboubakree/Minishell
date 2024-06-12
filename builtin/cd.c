/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 11:04:31 by akrid             #+#    #+#             */
/*   Updated: 2024/06/12 22:50:44 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *get_home( t_environment *env)
{
    t_environment *temp;
    char *home;

    temp = env_get_bykey(env, "HOME");
    home = NULL;
    if (temp)
        home = temp->value;
    return (home);
}

int cd_pars(t_minishell *singl_mini,char *path)
{
    if (path == NULL)
    {
        write(2, "bash: cd: HOME not set\n", 23);
        return (1);
    }
    if (args_count(singl_mini->args) > 2)
    {
        write(2,"bash: cd: too many arguments\n",29);
        return (1);
    }
    if (chdir(path))
    {
        if (errno == 116)
        {
            write(2, "bash: cd: ", ft_strlen("bash: cd: "));
            write(2, path, ft_strlen(path));
            write(2, ": No such file or directory\n", 28);
            return (1);
        }
        write(2, "bash: cd: ", ft_strlen("bash: cd: "));
        perror(path);
        return (1);
    }
    return (0);
}

void    handel_input_output(t_minishell *singl_mini)
{
    if (singl_mini->nbr_cmd == 1)
    {
        dup2(singl_mini->infile, 0);
        dup2(singl_mini->outfile, 1);
    }
    else
        get_in_out_priorities(singl_mini);
}


void set_pwd_oldpwd(t_environment *env, char *new_path)
{
    t_environment *OLDPWD;
    t_environment *PWD;

    OLDPWD = env_get_bykey(env, "OLDPWD");
    PWD = env_get_bykey(env, "PWD");
    if (OLDPWD && PWD)
    {
        free(OLDPWD->value);
        OLDPWD->value = PWD->value;
        PWD->value = ft_strdup(new_path);
    }
    else if (PWD)
    {
        if (PWD->value)
            free(PWD->value);
        PWD->value = ft_strdup(new_path);
    }
    else
    {
        if (OLDPWD->value)
            free(OLDPWD->value);
        OLDPWD->value = NULL;
    }
}

void cd(t_minishell *singl_mini, t_environment *env)
{
    char            new_path[1024];
    char            *path;

    if (open_files(singl_mini, env, singl_mini->files))
        return ;
    handel_input_output(singl_mini);
    if (args_count(singl_mini->args) == 1)
        path = get_home(env);
    else 
        path = singl_mini->args[1];
    if (path && ft_strncmp(path, "", 1) == 0 && args_count(singl_mini->args) <= 2)
        return (handel_exit_status(env, singl_mini->nbr_cmd, 0));
    if (cd_pars(singl_mini, path))
        return (handel_exit_status(env, singl_mini->nbr_cmd, 1));
    if (getcwd(new_path, 1024) == NULL)
    {
        printf("getcwd : %s: %s\n",strerror(errno), path);
        return (handel_exit_status(env, singl_mini->nbr_cmd, 1));
    }
    set_pwd_oldpwd(env, new_path);
    handel_exit_status(env, singl_mini->nbr_cmd, 0);
}
