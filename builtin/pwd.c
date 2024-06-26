/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 17:34:04 by akrid             #+#    #+#             */
/*   Updated: 2024/06/23 12:51:58 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pwd(t_minishell *singl_mini, t_environment *env)
{
	t_environment	*_pwd;
	char			new_path[1024];

	if (open_files(singl_mini, env, singl_mini->files))
		return ;
	handel_input_output(singl_mini);
	_pwd = env_get_bykey(env, "_pwd");
	if (_pwd && _pwd->value)
		printf("%s\n", _pwd->value);
	else
	{
		if (getcwd(new_path, 1024) == NULL)
		{
			perror("getcwd");
			return ;
		}
		printf("%s\n", new_path);
	}
	handel_exit_status(env, singl_mini->nbr_cmd, 0);
}
