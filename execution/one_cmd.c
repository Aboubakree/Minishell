/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 12:48:01 by akrid             #+#    #+#             */
/*   Updated: 2024/06/29 15:57:58 by akrid            ###   ########.fr       */
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

	handle_signals(SIG_IGN, SIG_IGN, interactive_sigint, SIG_IGN);
	pid = fork();
	if (pid == 0)
	{
		handle_signals(active_sigint, active_sigquit, SIG_IGN, SIG_IGN);
		start_execute_one(minishell, env);
	}
	wait(&status);
	unlink_files(minishell);
	if (status == 131 || status == 2)
	{
		if (status == 2)
			status += 128;
		if (status == 131)
			printf("Quit (core dumped)");
		set_exit_status(*env, status);
		printf("\n");
	}
	else
		set_exit_status(*env, WEXITSTATUS(status));
	handle_signals(interactive_sigint, SIG_IGN, SIG_IGN, SIG_IGN);
}
