/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions5.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtamouss <rtamouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 16:22:09 by rtamouss          #+#    #+#             */
/*   Updated: 2024/06/29 19:08:25 by rtamouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	puterr(char *str)
{
	ft_putstr_fd(str, 2);
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t'
		|| c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

int	count_char_occurence(char *str, int c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == c)
			count++;
		i++;
	}
	return (count);
}

void	skip_whitespaces(const char *str, int *i)
{
	while (str[*i] && is_whitespace(str[*i]))
		(*i)++;
}

int	ft_remove_char(char *str, int index)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (i == index)
			i++;
		str[j] = str[i];
		i++;
		j++;
	}
	str[j] = '\0';
	return (1);
}

// int	check_misplaced_operators_helper(const char *str, int *nb_quote_single
// 	, int *nb_quote_double,int *expect_operator)
// {
// 	int	i;

// 	i = 0;
// 	while (str[i])
// 	{
// 		update_nb_quote(str[i], nb_quote_single, nb_quote_double);
// 		if (str[i] == '|' && *nb_quote_single % 2 == 0
// 			&& *nb_quote_double % 2 == 0)
// 		{
// 			if (*expect_operator == 1)
// 				return (ft_putstr_fd("Syntax error
		// : Misplaced Operator\n", 2), 1);
// 			*expect_operator = 1;
// 		}
// 		else if (!is_whitespace(str[i]))
// 			*expect_operator = 0;
// 		i++;
// 	}
// 	return (0);
// }

// int	check_misplaced_operators(const char *str)
// {
// 	int	i;
// 	int	nb_quote_single;
// 	int	nb_quote_double;
// 	int	expect_operator;

// 	i = 0;
// 	nb_quote_single = 0;
// 	nb_quote_double = 0;
// 	expect_operator = 0;
// 	skip_whitespaces(str, &i);
// 	if (str[i] == '|')
// 		return (ft_putstr_fd("Syntax error : 
		//Misplaced Operator at first of line\n", 2), 1);
// 	if (check_misplaced_operators_helper(str, &nb_quote_single
// 			, &nb_quote_double, &expect_operator) == 1)
// 		return (1);
// 	if (expect_operator == 1)
// 		return (ft_putstr_fd("Syntax error : 
		//Misplaced Operator at end of line\n", 2), 1);
// 	else
// 		return (0);
// }

// int	check_invalid_redirection(const char *str)
// {
// 	int	i;
// 	int	nb_quote_single;
// 	int	nb_quote_double;

// 	i = 0;
// 	nb_quote_single = 0;
// 	nb_quote_double = 0;
// 	while (str[i])
// 	{
// 		update_nb_quote(str[i], &nb_quote_single, &nb_quote_double);
// 		if ((str[i] == '>' || str[i] == '<')
// 			&& nb_quote_single % 2 == 0 && nb_quote_double % 2 == 0)
// 		{
// 			if ((str[i] == '>' && str[i + 1] == '>')
// 				|| (str[i] == '<' && str[i + 1] == '<'))
// 				i++;
// 			if (str[i + 1] == '\0')
// 				return (printf("Syntax error : Invalid redirection\n"), 1);
// 			else if (str[i + 1] == '>' || str[i + 1] == '<')
// 				return (printf("Syntax error : Misplaced Operator\n"), 1);
// 		}
// 		i++;
// 	}
// 	return (0);
// }

// int	check_logical_operators(const char *str)
// {
// 	int	i;
// 	int	in_single_quote;
// 	int	in_double_quote;

// 	i = 0;
// 	in_single_quote = 0;
// 	in_double_quote = 0;
// 	if (!str)
// 		return (0);
// 	while (str[i])
// 	{
// 		if (str[i] == '\'' && !in_double_quote)
// 			in_single_quote = !in_single_quote;
// 		else if (str[i] == '\"' && !in_single_quote)
// 			in_double_quote = !in_double_quote;
// 		else if (str[i] == '&' && !in_single_quote && !in_double_quote)
// 		{
// 			while (str[i] && is_whitespace(str[i]))
// 				i++;
// 			if (str[i] == '&')
// 				return (puterr("Error: Logical Operators not supported.\n"), 1);
// 		}
// 		i++;
// 	}
// 	return (0);
// }
