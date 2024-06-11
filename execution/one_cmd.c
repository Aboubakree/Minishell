/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 12:48:01 by akrid             #+#    #+#             */
/*   Updated: 2024/06/11 13:17:58 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	file_error(t_minishell *minishell, t_environment *env, char *filename)
{
	write(2, "bash: ", 6);
	write(2, filename, ft_strlen(filename));
	write(2, ": ", 2);
	perror("");
	if (minishell->nbr_cmd == 1 && is_builtin(minishell->command))
	{
		set_exit_status(env, 1);
		return (1);
	}
	set_exit_status(env, 1);
	free_at_exit();
	exit(1);
}

int	open_file_extended(t_minishell *minishell, t_environment *env, t_file_redirection *files)
{
	if (files->type == T_REDIRECTION_APPEND)
	{
		if (minishell->outfile != 1)
			close(minishell->outfile);
		minishell->outfile = open(files->filename, O_WRONLY | O_APPEND | O_CREAT, 0664);
		if (minishell->outfile < 0)
			return (file_error(minishell, env, files->filename));
	}
	else if (files->type == T_HERDOC)
	{
		if (minishell->infile != 0)
			close(minishell->infile);
		minishell->infile = open(minishell->heredoc_path, O_RDONLY, 0644);
		if (minishell->infile < 0)
			return (file_error(minishell, env, "heredoc_buffer"));
	}
	return (0);
}

int	open_files(t_minishell *minishell,t_environment *env, t_file_redirection *files)
{
	files = minishell->files;
	while (files)
	{
		if (files->type == T_REDIRECTION_IN)
		{
			if (minishell->infile != 0)
				close(minishell->infile);
			minishell->infile = open(files->filename, O_RDONLY, 0644);
			if (minishell->infile < 0)
				return (file_error(minishell, env, files->filename));
		}
		else if (files->type == T_REDIRECTION_OUT)
		{
			if (minishell->outfile != 1)
				close(minishell->outfile);
			minishell->outfile = open(files->filename,
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (minishell->outfile < 0)
				return (file_error(minishell, env, files->filename));
		}
		else if (open_file_extended(minishell, env, files))
			return (1);
		files = files->next;
	}
	return (0);
}

void    start_execute_one(t_minishell *minishell, t_environment **env)
{
	char **env_conv;

	open_files(minishell, *env, minishell->files);
	if (minishell->command == NULL || ft_strncmp(minishell->command, "", 1) == 0)
	{
		free_at_exit();
		exit(0);
	}
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

void    execute_one(t_minishell *minishell, t_environment **env)
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