/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   manage_tokens.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rtamouss <rtamouss@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/28 20:11:26 by rtamouss      #+#    #+#                 */
/*   Updated: 2024/06/28 20:12:32 by rtamouss      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	tokens_size(t_token *tokens)
{
	int	size;

	size = 0;
	if (!tokens)
		return (0);
	while (tokens)
	{
		size++;
		tokens = tokens->next;
	}
	return (size);
}

t_token	get_last_token(t_token *tokens)
{
	t_token	*temp;

	temp = tokens;
	if (!temp)
		return (t_token){0,NULL,  NULL, NULL};
	while (temp->next)
		temp = temp->next;
	return (*temp);
}

void	remove_token(t_token **head, t_token *target)
{
	t_token	*temp;
	t_token	*prev;

	if (!*head)
		return ;
	if (*head == target)
	{
		if (target->next)
			target->next->prev = NULL;
		*head = target->next;
		free(target->value);
		free(target);
		return ;
	}
	find_target(head, target, &temp, &prev);
	if (temp)
	{
		if (temp->next)
			temp->next->prev = prev;
		prev->next = temp->next;
		free(temp->value);
		free(temp);
	}
}
t_token	*new_token(t_type_of_token type, char *value)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	token->next = NULL;
	token->prev = NULL;
	return (token);
}

void	add_token_front(t_token **head, t_token *new_token)
{
	new_token->prev = NULL;
	new_token->next = *head;
	*head = new_token;
}