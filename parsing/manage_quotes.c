/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   manage_quotes.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rtamouss <rtamouss@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/28 20:05:39 by rtamouss      #+#    #+#                 */
/*   Updated: 2024/06/28 20:08:40 by rtamouss      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_nb_quote(char c, int *nb_quote_single, int *nb_quote_double)
{
	if (c == '\'')
		*nb_quote_single += 1;
	if (c == '\"')
		*nb_quote_double += 1;
}


int	check_unclosed_quotes(const char *str)
{
	int	i;
	int	in_single_quotes;
	int	in_double_quotes;

	i = 0;
	if (!str)
		return (0);
	in_single_quotes = 0;
	in_double_quotes = 0;
	while (str[i])
	{
		if (str[i] == '\'' && in_double_quotes == 0)
			in_single_quotes = !in_single_quotes;
		if (str[i] == '\"' && in_single_quotes == 0)
			in_double_quotes = !in_double_quotes;
		i++;
	}
	return (in_single_quotes || in_double_quotes);
}


int	check_if_have_quotes(char *str)
{
	int	i;
	int	single_quotes;
	int	double_quotes;

	single_quotes = 0;
	double_quotes = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\"')
			double_quotes++;
		else if (str[i] == '\'')
			single_quotes++;
		i++;
	}
	if (single_quotes >= 1 || double_quotes >= 1)
		return (0);
	return (1);
}

void	update_quote_state(int *in_quote, char *quote, char c)
{
	if (*in_quote == 0 && (c == '\'' || c == '\"'))
	{
		*in_quote = 1;
		*quote = c;
	}
	else if (*in_quote == 1 && c == *quote)
		*in_quote = 0;
}

void check_quote(const char *str, int *in_single_quotes, int *in_double_quotes, int *i)
{
	if (str[*i] == '\'' && *in_single_quotes == 0 && *in_double_quotes == 0)
		*in_single_quotes = 1;
	else if (str[*i] == '\'' && *in_single_quotes == 1)
		*in_single_quotes = 0;
	else if (str[*i] == '\"' && *in_double_quotes == 0
		&& *in_single_quotes == 0)
		*in_double_quotes = 1;
	else if (str[*i] == '\"' && *in_double_quotes == 1)
		*in_double_quotes = 0;
}