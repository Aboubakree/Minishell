/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 09:11:41 by akrid             #+#    #+#             */
/*   Updated: 2024/05/24 11:59:11 by akrid            ###   ########.fr       */
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


void fake_exit(t_minishell *singl_mini)
{
    char **args;
    int  exit_status;

    open_files(singl_mini);
    get_in_out_priorities(singl_mini);
    args = singl_mini->args;
    exit_status = 0;
    if (args && args[1])
    {
        if (check_digits(args[1]) == 0)
        {
            printf("exit\nbash: exit: %s: numeric argument required\n", args[0]);
            exit(2);
            //$? = 2
        }
        if (args[2] != NULL)
        {
            //$? = 1
            return (printf("exit\nbash: exit: too many arguments\n"), (void)0);
        }
        exit_status = ft_atoi(args[1]);
    }
    printf("exit\n");
    exit(exit_status);
}
