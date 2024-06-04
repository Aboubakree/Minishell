/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:37:15 by akrid             #+#    #+#             */
/*   Updated: 2024/06/04 14:12:26 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int count_size(int nbr)
{
    int size;

    size = 0;
    if (nbr < 0)
        nbr = -nbr;
    while (nbr > 9)
    {
        size ++;
        nbr = nbr / 10;
    }
    size ++;
    return (size);
}

char *ft_putnbr(int nbr)
{
    char *to_str;
    int nbr_size;
    int ng;

    ng = 0;
    nbr_size = count_size(nbr);
    if (nbr < 0)
    {
        ng = 1;
        nbr = -nbr;
        nbr_size ++;
    }
    to_str = malloc(sizeof(char) * (nbr_size + ng + 1));
    to_str[nbr_size--] = '\0';
    while (nbr_size > 0)
    {
        to_str[nbr_size] = (nbr % 10) + '0';
        nbr = nbr / 10;
        nbr_size --;
    }
    to_str[nbr_size] = (nbr % 10) + '0';
    if (ng)
        to_str[0] = '-';
    return (to_str);
}

void set_exit_status(t_environment *env, int new_value)
{
    t_environment *EXIT;

    EXIT = env_get_bykey(env, "?");
    if (EXIT)
    {
        if (EXIT->value)
            free(EXIT->value);
        EXIT->value = ft_putnbr(new_value);
    }
}