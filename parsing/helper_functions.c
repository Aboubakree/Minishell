/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   helper_functions.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: rtamouss <rtamouss@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/28 21:00:28 by rtamouss      #+#    #+#                 */
/*   Updated: 2024/06/28 21:00:51 by rtamouss      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_operator(char c)
{
	return (c == '|' || c == '>' || c == '<');
}
int	is_pipe(char c)
{
	return (c == '|');
}
int	is_redirection_in(char c)
{
	return (c == '<');
}
int	is_redirection_out(char c)
{
	return (c == '>');
}

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}