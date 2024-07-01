/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 19:40:27 by akrid             #+#    #+#             */
/*   Updated: 2024/07/01 11:53:15 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_directory(char *cmd)
{
	struct stat	path_stat;

	if (stat(cmd, &path_stat) != 0)
		return (0);
	return (S_ISDIR(path_stat.st_mode));
}

int	empty_cmd(char *cmd)
{
	if (cmd == NULL)
	{
		free_at_exit();
		exit(0);
	}
	if (ft_strncmp(cmd, "", 1) == 0 || (ft_strlen(cmd) == 2 && cmd[0] == '"'
			&& cmd[1] == '"'))
	{
		write(2, "'': command not found\n", 22);
		return (free_at_exit(), 1);
	}
	if (ft_strncmp(cmd, ".", 2) == 0)
	{
		write(2, "bash: .: filename argument required\n", 36);
		free_at_exit();
		exit(2);
	}
	if (ft_strncmp(cmd, "..", 3) == 0)
	{
		write(2, "..: command not found\n", 22);
		free_at_exit();
		exit(127);
	}
	return (0);
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
