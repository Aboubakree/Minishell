/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: rtamouss <rtamouss@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/29 09:38:51 by rtamouss      #+#    #+#                 */
/*   Updated: 2024/07/02 18:41:26 by rtamouss      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_expand_string(int *i, int *in_single_quotes,
	int *in_double_quotes, char **env_variable)
{
	*i = 0;
	*in_single_quotes = 0;
	*in_double_quotes = 0;
	*env_variable = NULL;
}

char	*expand_string(char *str, int from_heredoc)
{
	int		i;
	int		in_single_quotes;
	int		in_double_quotes;
	int		index;
	char	*env_variable;

	init_expand_string(&i, &in_single_quotes, &in_double_quotes, &env_variable);
	while (str[i])
	{
		check_quote(str, &in_single_quotes, &in_double_quotes, &i);
		if ((str[i] == '$' && ((in_single_quotes == 0 && from_heredoc == 0
						&& is_word(str[i + 1])) || from_heredoc == 1)))
		{
			if (is_whitespace(str[i + 1]) || str[i + 1] == '\0')
			{
				i++;
				continue ;
			}
			index = expand_helper(index, &i, str);
			str = change_value(str, &i, &index, env_variable);
		}
		else
			i++;
	}
	return (str);
}

void	split_expanded_string(t_token **temp, t_token **tokens)
{
	char	**args;
	int		i;
	t_token	*temp_to_remove;

	args = ft_split2((*temp)->value, " \t\r");
	i = 0;
	while (args[i])
	{
		insert_token(tokens, new_token(T_WORD, ft_strdup(args[i])), (*temp));
		i++;
	}
	free_args(args);
	temp_to_remove = (*temp);
	(*temp) = (*temp)->next;
	remove_token(tokens, temp_to_remove);
}

int	expand_string_helper(t_token **tokens, t_token **temp)
{
	char	*old;
	char	*index_of_equal;
	char	*index_of_dollar;

	old = ft_strdup((*temp)->value);
	index_of_dollar = ft_strchr(old, '$');
	index_of_equal = ft_strchr(old, '=');
	(*temp)->value = expand_string((*temp)->value, 0);
	if (check_if_have_space((*temp)->value) == 1
		&& (index_of_equal == NULL || (index_of_equal >= index_of_dollar)))
	{
		if ((*temp)->prev != NULL)
		{
			if ((*temp)->prev->type == T_REDIRECTION_IN
				|| (*temp)->prev->type == T_REDIRECTION_OUT
				|| (*temp)->prev->type == T_REDIRECTION_APPEND)
				if (ambiguouse_redirect(old) == 1)
					return (free_tokens(*tokens), free(old), 1);
		}
		split_expanded_string(temp, tokens);
	}
	else
		(*temp) = (*temp)->next;
	free(old);
	return (0);
}

int check_if_dollars(char *str)
{
	if (str[0] == '$' && str[1] == '\0')
		return (1);
	return (0);
}

t_token	*expand(t_token *tokens)
{
	t_token	*temp;

	temp = NULL;
	temp = tokens;
	while (temp)
	{
		if (temp && temp->value)
		{
			if (temp->type == T_WORD)
			{
				if (ft_strchr(temp->value, '$') == NULL || (temp->prev != NULL && temp->prev->type == T_HERDOC))
				{
					temp = temp->next;
					continue ;
				}
				if (expand_string_helper(&tokens, &temp) == 1)
					return (NULL);
			}
			else
				temp = temp->next;
		}
	}
	return (tokens);
}
