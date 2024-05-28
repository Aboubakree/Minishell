/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 17:34:04 by akrid             #+#    #+#             */
/*   Updated: 2024/05/24 11:59:51 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    pwd(t_minishell *singl_mini, t_environment *env)
{
    t_environment *PWD;

    open_files(singl_mini);
    get_in_out_priorities(singl_mini);
    PWD = env_get_bykey(env, "PWD");
    if (PWD)
        printf("%s\n", PWD->value);
}