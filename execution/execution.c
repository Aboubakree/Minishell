/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 11:42:14 by akrid             #+#    #+#             */
/*   Updated: 2024/06/29 11:57:38 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	restore_std_in_out(int stdout, int stdin)
{
	dup2(stdin, STDIN_FILENO);
	dup2(stdout, STDOUT_FILENO);
	close(stdin);
	close(stdout);
}

char	*get_herdoc_path(int i)
{
	char	*heredoc_path;

	heredoc_path = malloc(sizeof(char) * (ft_strlen("/tmp/herdoc_buffer_")
				+ 2));
	ft_strlcpy(heredoc_path, "/tmp/herdoc_buffer_", 19);
	heredoc_path[ft_strlen("/tmp/herdoc_buffer_")] = i + 'a';
	heredoc_path[ft_strlen("/tmp/herdoc_buffer_") + 1] = '\0';
	return (heredoc_path);
}

void	minishell_init(t_minishell *minishell, int count_cmds)
{
	int			i;
	t_minishell	*temp;

	i = 0;
	temp = minishell;
	while (temp)
	{
		temp->cmd_order = i;
		temp->path = NULL;
		temp->pipe = NULL;
		temp->infile = 0;
		temp->outfile = 1;
		temp->nbr_cmd = count_cmds;
		temp->heredoc_path = get_herdoc_path(i);
		i++;
		temp = temp->next;
	}
}

void	set_under_score(t_minishell *minishell, t_environment **env)
{
	t_environment	*env_under_score;

	env_under_score = env_get_bykey(*env, "_");
	if (env_under_score)
	{
		if (args_count(minishell->args) > 0)
		{
			free(env_under_score->value);
			env_under_score->value
				= ft_strdup(minishell->args[args_count(minishell->args) - 1]);
		}
		else
		{
			free(env_under_score->value);
			env_under_score->value = ft_strdup("");
		}
	}
}

void	execution(t_minishell *minishell, t_environment **env)
{
	int	stdout;
	int	stdin;

	stdout = dup(STDOUT_FILENO);
	stdin = dup(STDIN_FILENO);
	minishell_init(minishell, cmd_count(minishell));
	if (check_heredoc(minishell, *env, 0))
		return ;
	if (cmd_count(minishell) == 1)
	{
		if (check_builtin(minishell, env) == 0)
		{
			set_under_score(minishell, env);
			restore_std_in_out(stdout, stdin);
			return ;
		}
		else
			execute_one(minishell, env);
		set_under_score(minishell, env);
	}
	else
		execute_all(minishell, env);
	restore_std_in_out(stdout, stdin);
}
