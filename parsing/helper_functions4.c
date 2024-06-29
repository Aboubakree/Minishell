/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions4.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtamouss <rtamouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 16:20:41 by rtamouss          #+#    #+#             */
/*   Updated: 2024/06/29 19:22:02 by rtamouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_word(char c)
{
	return (!is_arithmetic_operator(c)
		&& !is_colone(c)
		&& !is_operator(c)
		&& !is_quote(c)
		&& !is_whitespace(c)
		&& !is_env_variable(c)
		&& !not_alpha_numeric(c));
}

void	find_target(t_token **head, t_token *target,
	t_token **temp, t_token **prev)
{
	*temp = *head;
	while (*temp && *temp != target)
	{
		*prev = *temp;
		*temp = (*temp)->next;
	}
}

int	check_whitespaces(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (is_whitespace(str[i]) != 1)
			return (1);
		i++;
	}
	return (0);
}

void	handle_quotes_after_dollar(char *str)
{
	int	i;

	if (str[0] == '$' && (str[1] == '\'' || str[1] == '\"'))
	{
		i = 0;
		while (str[i])
		{
			str[i] = str[i + 1];
			i++;
		}
	}
}

int	ambiguouse_redirect(char *old)
{
	if (old[0] != '\"' && old[ft_strlen(old) - 1] != '\"')
	{
		puterr("minishell: ");
		puterr(old);
		puterr(": ambiguous redirect\n");
		set_exit_status(*(lists_collecter->env), 1);
		return (1);
	}
	return (0);
}
