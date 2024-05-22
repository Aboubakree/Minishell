/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 17:34:04 by akrid             #+#    #+#             */
/*   Updated: 2024/05/21 14:35:15 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void    pwd(t_environment *env)
{
    t_environment *PWD;

    PWD = env_get_bykey(env, "PWD");
    if (PWD)
        printf("%s\n", PWD->value);
}