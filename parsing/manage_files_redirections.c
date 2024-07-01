/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_files_redirections.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtamouss <rtamouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 21:03:49 by rtamouss          #+#    #+#             */
/*   Updated: 2024/06/29 19:22:47 by rtamouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_file_redirection	*new_file_redirection(char *filename, t_type_of_token type)
{
	t_file_redirection	*file;

	file = (t_file_redirection *)malloc(sizeof(t_file_redirection));
	if (!file)
		return (NULL);
	file->filename = filename;
	file->should_expand_heredoc = 1;
	file->should_expand_heredoc = check_if_have_quotes(filename);
	file->type = type;
	file->next = NULL;
	return (file);
}

void	add_file_redirection_back(t_file_redirection **head
	, t_file_redirection *new_file)
{
	t_file_redirection	*temp;

	if (!*head)
		*head = new_file;
	else
	{
		temp = *head;
		while (temp->next)
			temp = temp->next;
		temp->next = new_file;
	}
}

void	print_files(t_minishell *temp)
{
	t_file_redirection	*temp_files;

	if (temp->files)
	{
		temp_files = temp->files;
		printf("files :\n");
		while (temp_files)
		{
			printf(" file: [%s] \n", temp_files->filename);
			printf("  type: %s \n", get_type_token(temp_files->type));
			temp_files = temp_files->next;
		}
		printf("\n");
	}
}
