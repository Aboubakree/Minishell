/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtamouss <rtamouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 21:32:20 by rtamouss          #+#    #+#             */
/*   Updated: 2024/06/29 19:26:58 by rtamouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_environment(t_environment *env)
{
	t_environment	*temp;

	if (!env)
		return ;
	while (env)
	{
		temp = env;
		env = env->next;
		free(temp->key);
		free(temp->value);
		free(temp);
	}
}

void	free_files(t_file_redirection *files)
{
	t_file_redirection	*temp_files;
	t_file_redirection	*temp_files_next;

	if (files)
	{
		temp_files = files;
		while (temp_files)
		{
			temp_files_next = temp_files->next;
			free(temp_files->filename);
			free(temp_files);
			temp_files = temp_files_next;
		}
	}
}

void	free_minishell(t_minishell *minishell)
{
	t_minishell			*temp;
	char				**temp_args;

	if (!minishell)
		return ;
	if (minishell->pipe)
		free(minishell->pipe);
	while (minishell)
	{
		temp = minishell;
		minishell = minishell->next;
		if (temp->command)
			free(temp->command);
		if (temp->heredoc_path)
			free(temp->heredoc_path);
		if (temp->path)
			free(temp->path);
		if (temp->args)
		{
			temp_args = temp->args;
			free_args(temp_args);
		}
		free_files(temp->files);
		free(temp);
	}
}

void	free_heredocs(t_file_redirection *files)
{
	t_file_redirection	*temp;

	while (files)
	{
		temp = files;
		files = files->next;
		free(temp->filename);
		free(temp);
	}
}

void	free_at_exit(void)
{
	if (*lists_collecter->env)
		free_environment(*lists_collecter->env);
	if (*lists_collecter->minishell)
		free_minishell(*lists_collecter->minishell);
	if (*lists_collecter->tokens)
		free_tokens(*lists_collecter->tokens);
	free(lists_collecter);
}
