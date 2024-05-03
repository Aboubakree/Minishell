/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 09:11:41 by akrid             #+#    #+#             */
/*   Updated: 2024/05/02 14:38:51 by akrid            ###   ########.fr       */
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


void fake_exit(char *arg)
{
    char **args;
    int  exit_status;


    args = ft_split(arg, ' ');
    exit_status = 0;
    if (args && args[0])
    {
        if (check_digits(args[0]) == 0)
        {
            printf("exit\nbash: exit: %s: numeric argument required\n", args[0]);
            exit(2);
        }
        if (args[1] != NULL)
        {
            //$? = 1
            return (printf("exit\nbash: exit: too many arguments\n"), (void)0);
        }
        exit_status = ft_atoi(args[0]);
    }
    printf("exit\n");
    exit(exit_status);
}
