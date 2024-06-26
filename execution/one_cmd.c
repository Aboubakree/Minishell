/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 12:48:01 by akrid             #+#    #+#             */
/*   Updated: 2024/06/26 18:20:16 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	start_execute_one(t_minishell *minishell, t_environment **env)
{
	char	**env_conv;

	open_files(minishell, *env, minishell->files);
	minishell->path = get_cmd_path(minishell->command, *env, 0);
	if (minishell->path == NULL)
	{
		free_at_exit();
		exit(127);
	}
	dup2(minishell->infile, 0);
	dup2(minishell->outfile, 1);
	env_conv = convert_env(*env);
	execve(minishell->path, minishell->args, env_conv);
	free_split(env_conv);
	free_at_exit();
	perror("execve");
	exit(126);
}

void	execute_one(t_minishell *minishell, t_environment **env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
		start_execute_one(minishell, env);
	wait(&status);
	unlink_files(minishell);
	status = status >> 8;
	set_exit_status(*env, status);
}
