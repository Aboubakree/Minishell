/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 18:48:43 by akrid             #+#    #+#             */
/*   Updated: 2024/05/02 09:04:16 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    envi(t_environment *env)
{
    t_environment *temp;

    temp = env;
    while (temp)
    {
        if (temp->value != NULL)
            printf("%s=%s\n", temp->key, temp->value);
        temp = temp->next;
    }
}
