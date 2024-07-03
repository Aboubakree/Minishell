/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_error2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 16:17:39 by rtamouss          #+#    #+#             */
/*   Updated: 2024/07/03 23:05:18 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	fill_heredoc_for_sr(t_file_redirection *heredoc)
{
	char	*str;

	while (1)
	{
		str = readline("> ");
		if (check_delemeter_for_sr(str, heredoc->filename) == 1)
			break ;
		if (str)
			free(str);
	}
}

void	loop_heredoc_for_sr(t_file_redirection *heredocs)
{
	t_file_redirection	*temp;

	temp = heredocs;
	while (temp)
	{
		fill_heredoc_for_sr(temp);
		temp = temp->next;
	}
}

void	fill_error_codes(char **error_codes)
{
	error_codes[0] = "";
	error_codes[1]
		= "minishell: syntax error near unexpected token `newline'\n";
	error_codes[2]
		= "minishell: syntax error near unexpected token `|'\n";
	error_codes[3]
		= "minishell: syntax error near unexpected token `newline'\n";
	error_codes[4]
		= "minishell: syntax error near unexpected token `<<'\n";
	error_codes[5]
		= "minishell: syntax error near unexpected token `<'\n";
	error_codes[6]
		= "minishell: syntax error near unexpected token `>'\n";
	error_codes[7]
		= "minishell: syntax error near unexpected token `>>'\n";
	error_codes[8]
		= "Error: Logical operators not supported YET.\n";
	error_codes[9]
		= "minishell: syntax error near unexpected token `<<'\n";
}

int	help_heredoc_sr(int *heredoc_counter, t_token *tokens,
	int error_code, t_file_redirection **heredocs)
{
	t_token		*temp;
	char		*error_codes[10];
	int			print_error;

	temp = tokens;
	fill_error_codes(error_codes);
	print_error = 1;
	while (temp)
	{
		if (temp->type == T_HERDOC && temp->next != NULL)
			(add_file_redirection_back(heredocs, new_file_redirection(\
				ft_strdup(temp->next->value), T_HERDOC)), (*heredoc_counter)++);
		if (temp->type == T_PIPE)
		{
			set_exit_status(*((get_list_collecter())->env), 2);
			puterr(error_codes[error_code]);
			print_error = 0;
			if (*heredoc_counter == 0)
				return (-1);
		}
		temp = temp->next;
	}
	return (print_error);
}

int	check_heredoc_for_syntax_error(t_file_redirection **heredocs
	, t_token *tokens, int error_code)
{
	char	*error_codes[10];
	int		print_error;
	int		heredoc_counter;

	heredoc_counter = 0;
	fill_error_codes(error_codes);
	if (error_code == 3 || error_code == 9)
		return (puterr(error_codes[error_code]), 0);
	print_error = help_heredoc_sr(&heredoc_counter,
			tokens, error_code, heredocs);
	if (print_error == -1)
		return (0);
	set_exit_status(*((get_list_collecter())->env), 2);
	if (heredoc_counter == 0)
		return (puterr(error_codes[error_code]), 0);
	if (heredoc_counter > 0 && heredoc_counter < 17)
	{
		loop_heredoc_for_sr(*heredocs);
		if (print_error == 1)
			puterr(error_codes[error_code]);
		return (0);
	}
	else if (heredoc_counter > 16)
		return (puterr("minishell: maximum here-document count exceeded\n"), 0);
	return (1);
}
