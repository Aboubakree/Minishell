/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_cmds.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 12:01:15 by akrid             #+#    #+#             */
/*   Updated: 2024/07/01 11:52:14 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_last_exitstat(t_environment *env, int status, int *new_line)
{
	if (status == 131 || status == 2)
	{
		if (status == 2)
			status += 128;
		if (status == 131)
			printf("Quit (core dumped)");
		set_exit_status(env, status);
	}
	else
	{
		set_exit_status(env, WEXITSTATUS(status));
	}
	if (status < 200 && status != 0 && *new_line)
	{
		*new_line = 0;
		printf("\n");
	}
}

void	wait_childs(t_minishell *mini, t_environment *env,
	int num_cmd, pid_t last_pid)
{
	int		i;
	int		status;
	pid_t	pid;
	int		new_line;

	i = 0;
	new_line = 1;
	close_pipes(mini->pipe, mini->nbr_cmd);
	while (i < num_cmd)
	{
		status = 0;
		pid = wait(&status);
		if (pid == last_pid)
			set_last_exitstat(env, status, &new_line);
		if (status == 2 && pid != last_pid && new_line)
		{
			new_line = 0;
			printf("\n");
		}
		i++;
	}
	unlink_files(mini);
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
	wait_childs(minishell, *env, minishell->nbr_cmd, pid);
	handle_signals(interactive_sigint, SIG_IGN, SIG_IGN, SIG_IGN);
}
