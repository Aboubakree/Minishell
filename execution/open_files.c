/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 22:18:35 by akrid             #+#    #+#             */
/*   Updated: 2024/06/12 22:28:08 by akrid            ###   ########.fr       */
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

int	check_std_in_out(t_type_of_token type, char *filename)
{
	if (ft_strncmp(filename, "/dev/stdout", ft_strlen(filename) + 1) == 0
		&& (type == T_REDIRECTION_OUT || type == T_REDIRECTION_APPEND))
		return (2);
	if (ft_strncmp(filename, "/dev/stdin", ft_strlen(filename) + 1) == 0
		&& type == T_REDIRECTION_IN)
		return (2);
	return (0);
}

int	open_file_extended_2(t_minishell *minishell, t_environment *env,
		t_file_redirection *files)
{
	if (check_std_in_out(files->type, files->filename))
		return (2);
	if (files->type == T_REDIRECTION_APPEND)
	{
		if (minishell->outfile != 1)
			close(minishell->outfile);
		minishell->outfile = open(files->filename,
				O_WRONLY | O_APPEND | O_CREAT, 0664);
		if (minishell->outfile < 0)
			return (file_error(minishell, env, files->filename));
	}
	else if (files->type == T_HERDOC)
	{
		if (minishell->infile != 0)
			close(minishell->infile);
		minishell->infile = open(minishell->heredoc_path, O_RDONLY, 0644);
		if (minishell->infile < 0)
			return (file_error(minishell, env, minishell->heredoc_path));
	}
	return (0);
}

int	open_file_extended(t_minishell *minishell, t_environment *env,
		t_file_redirection *files)
{
	if (check_std_in_out(files->type, files->filename))
		return (2);
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
		minishell->outfile = open(files->filename, O_WRONLY | O_CREAT | O_TRUNC,
				0644);
		if (minishell->outfile < 0)
			return (file_error(minishell, env, files->filename));
	}
	return (0);
}

int	open_files(t_minishell *minishell, t_environment *env,
		t_file_redirection *files)
{
	int	return_value;

	files = minishell->files;
	while (files)
	{
		return_value = open_file_extended(minishell, env, files);
		if (return_value == 1)
			return (1);
		else if (return_value == 0)
		{
			return_value = open_file_extended_2(minishell, env, files);
			if (return_value == 1)
				return (1);
		}
		files = files->next;
	}
	return (0);
}
