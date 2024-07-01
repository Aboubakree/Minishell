/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   delete_quotes.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rtamouss <rtamouss@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/29 16:08:13 by rtamouss      #+#    #+#                 */
/*   Updated: 2024/06/30 19:45:33by rtamouss      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	delete_quotes_from_string_helper(char *str, int *k,
	int *in_single_quote, int *in_double_quote)
{
	int	j;

	j = 0;
	while (str[j])
	{
		if (str[j] == '\'' && !*in_double_quote)
			*in_single_quote = !*in_single_quote;
		else if (str[j] == '\"' && !*in_single_quote)
			*in_double_quote = !*in_double_quote;
		else if (str[j] == '$'
			&& *in_double_quote == 0 && *in_single_quote == 0 && str[j+1] != '\0' && !is_alpha(str[j+1]))
		{
			if (str[j] == '\'' && !*in_double_quote)
				*in_single_quote = !*in_single_quote;
			else if (str[j] == '\"' && !*in_single_quote)
				*in_double_quote = !*in_double_quote;
		}
		else
		{
			str[*k] = str[j];
			(*k)++;
		}
		j++;
	}
}

void	delete_quotes_from_string(char *str)
{
	int	in_single_quote;
	int	in_double_quote;
	int	k;

	in_single_quote = 0;
	in_double_quote = 0;
	k = 0;
	delete_quotes_from_string_helper(str,
		&k, &in_single_quote, &in_double_quote);
	str[k] = '\0';
}

void	delete_quotes_from_files(t_minishell *minishell)
{
	t_minishell			*temp;
	t_file_redirection	*files;

	temp = minishell;
	if (temp != NULL)
	{
		while (temp)
		{
			files = temp->files;
			while (files)
			{
				delete_quotes_from_string(files->filename);
				files = files->next;
			}
			temp = temp->next;
		}
	}
}

void	delete_quotes_from_args(t_minishell *minishell)
{
	t_minishell	*temp;
	int			i;

	i = 0;
	temp = minishell;
	if (temp->args != NULL)
	{
		while (temp && temp->args != NULL)
		{
			i = 0;
			while (temp->args[i])
			{
				delete_quotes_from_string(temp->args[i]);
				i++;
			}
			temp = temp->next;
		}
		if (minishell->command)
		{
			free(minishell->command);
			minishell->command = NULL;
		}
		if (minishell->args[0])
			minishell->command = ft_strdup(minishell->args[0]);
	}
}

t_minishell	*delete_quotes(t_minishell *minishell)
{
	t_minishell			*temp;

	temp = minishell;
	delete_quotes_from_args(temp);
	temp = minishell;
	delete_quotes_from_files(temp);
	return (minishell);
}
