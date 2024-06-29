/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_cmds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 12:01:15 by akrid             #+#    #+#             */
/*   Updated: 2024/06/29 17:18:19 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	wait_childs(t_minishell *mini, t_environment *env, int num_cmd)
{
	int	i;
	int	status;

	i = 0;
	close_pipes(mini->pipe, mini->nbr_cmd);
	while (i < num_cmd)
	{
		status = 0;
		wait(&status);
		if (status == 131 || status == 2)
		{
			if (status == 2)
				status += 128;
			if (status == 131)
				printf("Quit (core dumped)");
			set_exit_status(env, status);
		}
		else
			set_exit_status(env, WEXITSTATUS(status));
		i++;
	}
	if (status < 255 && status != 0)
		printf("\n");
	unlink_files(mini);
}

void	get_in_out_extended(t_minishell *singl_mini)
{
	if (singl_mini->infile == 0)
		dup2(singl_mini->pipe[(singl_mini->cmd_order - 1) * 2], 0);
	else
		dup2(singl_mini->infile, 0);
	if (singl_mini->outfile == 1)
		dup2(singl_mini->pipe[singl_mini->cmd_order * 2 + 1], 1);
	else
		dup2(singl_mini->outfile, 1);
}

void	get_in_out_priorities(t_minishell *singl_mini)
{
	if (singl_mini->cmd_order == 0)
	{
		dup2(singl_mini->infile, STDIN_FILENO);
		if (singl_mini->outfile == STDOUT_FILENO)
			dup2(singl_mini->pipe[1], STDOUT_FILENO);
		else
			dup2(singl_mini->outfile, 1);
	}
	else if (singl_mini->cmd_order == singl_mini->nbr_cmd - 1)
	{
		if (singl_mini->infile == 0)
			dup2(singl_mini->pipe[(singl_mini->cmd_order - 1) * 2], 0);
		else
			dup2(singl_mini->infile, 0);
		dup2(singl_mini->outfile, 1);
	}
	else
		get_in_out_extended(singl_mini);
	close_pipes(singl_mini->pipe, singl_mini->nbr_cmd);
}

void	final_execution(t_minishell *singl_mini, t_environment **env)
{
	char	**env_conv;

	open_files(singl_mini, *env, singl_mini->files);
	get_in_out_priorities(singl_mini);
	if (check_builtin(singl_mini, env) == 0)
	{
		free_at_exit();
		exit(0);
	}
	singl_mini->path = get_cmd_path(singl_mini->command, *env, 0);
	if (singl_mini->path == NULL)
	{
		free_at_exit();
		exit(127);
	}
	env_conv = convert_env(*env);
	execve(singl_mini->path, singl_mini->args, env_conv);
	perror("bash");
	free_split(env_conv);
	free_at_exit();
	exit(126);
}

void	execute_all(t_minishell *minishell, t_environment **env)
{
	pid_t		pid;
	t_minishell	*temp;

	pipe_init(minishell);
	temp = minishell;
	handle_signals(SIG_IGN, SIG_IGN, interactive_sigint, SIG_IGN);
	while (temp)
	{
		pid = fork();
		if (pid == 0)
		{
			handle_signals(active_sigint, active_sigquit, SIG_IGN, SIG_IGN);
			final_execution(temp, env);
		}
		temp = temp->next;
	}
	wait_childs(minishell, *env, minishell->nbr_cmd);
	handle_signals(interactive_sigint, SIG_IGN, SIG_IGN, SIG_IGN);
}
