/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 17:34:04 by akrid             #+#    #+#             */
/*   Updated: 2024/06/04 14:27:05 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    pwd(t_minishell *singl_mini, t_environment *env)
{
    t_environment *PWD;
    char            new_path[1024];

    open_files(singl_mini);
    handel_input_output(singl_mini);
    PWD = env_get_bykey(env, "PWD");
    if (PWD)
        printf("%s\n", PWD->value);
    else
    {
        if (getcwd(new_path, 1024) == NULL)
        {
            perror("getcwd");
            return ;
        }
        printf("%s\n", new_path);
    }
    handel_exit_status(env, singl_mini->nbr_cmd, 0);
}
