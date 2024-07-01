/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_to_minishell_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtamouss <rtamouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 16:15:51 by rtamouss          #+#    #+#             */
/*   Updated: 2024/06/29 19:19:42 by rtamouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*add_redirection_in(t_token *temp, t_file_redirection **files)
{
	temp = temp->next;
	add_file_redirection_back(files, new_file_redirection(\
		ft_strdup(temp->value), T_REDIRECTION_IN));
	return (temp);
}

t_token	*add_redirection_out(t_token *temp, t_file_redirection **files)
{
	temp = temp->next;
	add_file_redirection_back(files, new_file_redirection(\
		ft_strdup(temp->value), T_REDIRECTION_OUT));
	return (temp);
}

t_token	*add_redirection_append(t_token *temp, t_file_redirection **files)
{
	temp = temp->next;
	add_file_redirection_back(files, new_file_redirection(\
		ft_strdup(temp->value), T_REDIRECTION_APPEND));
	return (temp);
}

t_token	*add_heredoc(t_token *temp, t_file_redirection **files)
{
	temp = temp->next;
	add_file_redirection_back(files, new_file_redirection(\
		ft_strdup(temp->value), T_HERDOC));
	return (temp);
}

int	initialize_and_free(t_minishell_data_help *data)
{
	data->command = NULL;
	free(data->args1);
	data->args = NULL;
	data->args1 = NULL;
	data->files = NULL;
	return (1);
}
