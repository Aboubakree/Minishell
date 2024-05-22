/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 11:04:31 by akrid             #+#    #+#             */
/*   Updated: 2024/05/22 10:30:59 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char *get_home( t_environment *env)
{
    t_environment *temp;
    static char *home = NULL;

    if (home == NULL)
    {
        temp = env_get_bykey(env, "HOME");
        if (temp)
            home = ft_strdup(temp->value);
    }
    return (home);
    // dont forgot to free home ???
}

int cd_pars(t_minishell *singl_mini,char *path)
{
    if (args_count(singl_mini->args) > 2)
    {
        printf("bash: cd: too many arguments\n");
        // ? = 1
        return (1);
    }
    if (chdir(path))
    {
        printf("bash: cd: %s: No such file or directory\n", path);
        // ? == 1
        return (1);
    }
    return (0);
}

void cd(t_minishell *singl_mini, t_environment *env)
{
    t_environment *OLDPWD;
    t_environment *PWD;
    char            new_path[1024];
    char            *path;

    OLDPWD = env_get_bykey(env, "OLDPWD");
    PWD = env_get_bykey(env, "PWD");
    if (args_count(singl_mini->args) == 1) // complete if cd without args 
        path = get_home(env);
    else 
        path = singl_mini->args[1];
    if (cd_pars(singl_mini, path))
        return ;
    if (getcwd(new_path, 1024) == NULL)
    {
        printf("getcwd : %s: %s\n",strerror(errno), path);
        return ;
    }
    if (OLDPWD && PWD)
    {
        free(OLDPWD->value);
        OLDPWD->value = PWD->value;
        PWD->value = ft_strdup(new_path);
    }
}