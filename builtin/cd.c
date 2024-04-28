/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 11:04:31 by akrid             #+#    #+#             */
/*   Updated: 2024/04/28 16:31:14 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    cd(char *path, t_environment *env)
{
    t_environment *OLDPWD;
    t_environment *PWD;
    char            new_path[1024];

    if (chdir(ft_substr(path, 0, ft_strlen(path))))
        printf("bash: cd: %s: No such file or directory\n", path);
    else 
    {
        OLDPWD = env_get_bykey(env, "OLDPWD");
        PWD = env_get_bykey(env, "PWD");
        if (getcwd(new_path, 1024) == NULL)
            printf("getcwd : %s: %s\n",strerror(errno), path);
        else
        {
            free(OLDPWD->value);
            OLDPWD->value = PWD->value;
            PWD->value = ft_strdup(new_path);
        }
    }
}
