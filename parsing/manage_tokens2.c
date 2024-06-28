/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   manage_tokens2.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: rtamouss <rtamouss@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/28 20:12:47 by rtamouss      #+#    #+#                 */
/*   Updated: 2024/06/28 20:13:11 by rtamouss      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void insert_token(t_token **head, t_token *new_token, t_token *target)
{
	t_token	*temp;

	if (!*head)
		*head = new_token;
	else
	{
		temp = *head;
		while (temp->next && temp->next != target)
			temp = temp->next;
		new_token->next = temp->next;
		if (temp->next)
			temp->next->prev = new_token;
		new_token->prev = temp;
		temp->next = new_token;
	}
}

void	add_token_back(t_token **head, t_token *new_token)
{
	t_token	*temp;

	if (!*head)
		*head = new_token;
	else
	{
		temp = *head;
		while (temp->next)
			temp = temp->next;
		temp->next = new_token;
		new_token->prev = temp;
	}
}


char	*get_type_token(t_type_of_token type)
{
	if (type == T_WORD)
		return ("T_WORD");
	else if (type == T_PIPE)
		return ("T_PIPE");
	else if (type == T_REDIRECTION_IN)
		return ("T_REDIRECTION_IN");
	else if (type == T_REDIRECTION_OUT)
		return ("T_REDIRECTION_OUT");
	else if (type == T_REDIRECTION_APPEND)
		return ("T_REDIRECTION_APPEND");
	else if (type == T_HERDOC)
		return ("T_HERDOC");
	return ("UNKNOWN");
}

void	print_tokens(t_token *tokens)
{
	printf("--------------------\n");
	while (tokens)
	{
		printf("type: %s, value: [%s]\n"
			, get_type_token(tokens->type), tokens->value);
		tokens = tokens->next;
	}
	printf("--------------------\n");
}