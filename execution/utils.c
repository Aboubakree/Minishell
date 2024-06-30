/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 12:55:10 by akrid             #+#    #+#             */
/*   Updated: 2024/06/29 20:07:07 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	args_count(char **args)
{
	int	i;

	i = 0;
	while (args && args[i])
		i++;
	return (i);
}

int	check_builtin(t_minishell *singl_mini, t_environment **env)
{
	if (singl_mini->command == NULL)
		return (1);
	if (ft_strncmp("cd", singl_mini->command, 3) == 0)
		return (cd(singl_mini, *env), 0);
	if (ft_strncmp("echo", singl_mini->command, 5) == 0)
		return (echo(singl_mini, *env), 0);
	if (ft_strncmp("env", singl_mini->command, 4) == 0)
		return (envi(singl_mini, *env), 0);
	if (ft_strncmp("pwd", singl_mini->command, 4) == 0)
		return (pwd(singl_mini, *env), 0);
	if (ft_strncmp("export", singl_mini->command, 7) == 0)
		return (export(singl_mini, env), 0);
	if (ft_strncmp("unset", singl_mini->command, 6) == 0)
		return (unset(singl_mini, env), 0);
	if (strncmp("exit", singl_mini->command, 5) == 0)
		return (fake_exit(singl_mini, *env), 0);
	return (1);
}

int	cmd_count(t_minishell *minishell)
{
	int	count;

	count = 0;
	while (minishell)
	{
		count++;
		minishell = minishell->next;
	}
	return (count);
}

int	is_builtin(char *cmd)
{
	if (cmd == NULL)
		return (0);
	if (ft_strncmp("cd", cmd, 3) == 0)
		return (1);
	if (ft_strncmp("echo", cmd, 5) == 0)
		return (1);
	if (ft_strncmp("env", cmd, 4) == 0)
		return (1);
	if (ft_strncmp("pwd", cmd, 4) == 0)
		return (1);
	if (ft_strncmp("export", cmd, 7) == 0)
		return (1);
	if (ft_strncmp("unset", cmd, 6) == 0)
		return (1);
	if (strncmp("exit", cmd, 5) == 0)
		return (1);
	return (0);
}
