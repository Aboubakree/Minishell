/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 09:11:41 by akrid             #+#    #+#             */
/*   Updated: 2024/06/04 14:21:21 by akrid            ###   ########.fr       */
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
    char **args;
    int  exit_status;

    open_files(singl_mini);
    handel_input_output(singl_mini);
    args = singl_mini->args;
    exit_status = 0;
    if (args && args[1])
    {
        if (check_digits(args[1]) == 0)
        {
            printf("exit\nbash: exit: %s: numeric argument required\n", args[1]);
            exit(2);
        }
        if (args[2] != NULL)
        {
            set_exit_status(env, 1);
            return (printf("exit\nbash: exit: too many arguments\n"), (void)0);
        }
        exit_status = ft_atoi(args[1]);
    }
    printf("exit\n");
    exit(exit_status);
}
