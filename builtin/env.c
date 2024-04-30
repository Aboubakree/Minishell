/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 18:48:43 by akrid             #+#    #+#             */
/*   Updated: 2024/04/29 18:58:09 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    envi(t_environment *env)
{
    t_environment *temp;

    temp = env;
    while (temp)
    {
        printf("%s=%s\n", temp->key, temp->value);
        temp = temp->next;
    }
}