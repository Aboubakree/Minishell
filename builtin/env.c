/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 18:48:43 by akrid             #+#    #+#             */
/*   Updated: 2024/05/28 16:12:02 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    envi(t_minishell *singl_mini, t_environment *env)
{
    t_environment *temp;

    open_files(singl_mini);
    handel_input_output(singl_mini);
    temp = env;
    while (temp)
    {
        if (temp->value != NULL)
            printf("%s=%s\n", temp->key, temp->value);
        temp = temp->next;
    }
}
