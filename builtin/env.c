/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 18:48:43 by akrid             #+#    #+#             */
/*   Updated: 2024/06/04 14:14:09 by akrid            ###   ########.fr       */
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
        if (temp->value != NULL && ft_strncmp(temp->key, "?", 2) != 0)
        {
            if (ft_strncmp(temp->key, "_", 2) == 0)
                printf("%s=%s\n", temp->key, "/usr/bin/env");
            else
                printf("%s=%s\n", temp->key, temp->value);
        }
        temp = temp->next;
    }
    handel_exit_status(env, singl_mini->nbr_cmd, 0);
}
