/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 09:11:41 by akrid             #+#    #+#             */
/*   Updated: 2024/06/08 11:24:51 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int check_digits(char *str)
{
    int i;

    i = 0;
    while (str && str[i])
    {
        if (ft_isdigit(str[i]) == 0)
            return (0);
        i ++;
    }
    return (1);
}


void fake_exit(t_minishell *singl_mini, t_environment *env)
{
    int  exit_status;

    if (open_files(singl_mini, env, singl_mini->files))
        return ;
    handel_input_output(singl_mini);
    exit_status = 0;
    if (singl_mini->args && singl_mini->args[1])
    {
        if (check_digits(singl_mini->args[1]) == 0)
        {
            printf("exit\nbash: exit: %s: numeric argument required\n", singl_mini->args[1]);
            free_at_exit();
            exit(2);
        }
        if (singl_mini->args[2] != NULL)
        {
            set_exit_status(env, 1);
            return (printf("exit\nbash: exit: too many arguments\n"), (void)0);
        }
        exit_status = ft_atoi(singl_mini->args[1]);
    }
    printf("exit\n");
    free_at_exit();
    exit(exit_status);
}
