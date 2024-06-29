/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtamouss <rtamouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:18:34 by akrid             #+#    #+#             */
/*   Updated: 2024/06/10 20:13:09 rtamouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
// syntax error checking/

t_lists_collecter *lists_collecter;

void	puterr(char *str)
{
	ft_putstr_fd(str, 2);
}
int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t'
		|| c == '\n' || c == '\v' || c == '\f' || c == '\r');
}
int	check_if_have_space(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (is_whitespace(str[i]) == 1)
			return (1);
		i++;
	}
	return (0);
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

void skip_whitespaces(const char *str, int *i)
{
	while (str[*i] && is_whitespace(str[*i]))
		(*i)++;
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
// 				return (ft_putstr_fd("Syntax error : Misplaced Operator\n", 2), 1);
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
// 		return (ft_putstr_fd("Syntax error : Misplaced Operator at first of line\n", 2), 1);
// 	if (check_misplaced_operators_helper(str, &nb_quote_single
// 			, &nb_quote_double, &expect_operator) == 1)
// 		return (1);
// 	if (expect_operator == 1)
// 		return (ft_putstr_fd("Syntax error : Misplaced Operator at end of line\n", 2), 1);
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


// syntax error check
// tokenization



int not_alpha_numeric(char c)
{
	return (!is_alpha(c) && !is_number(c) && c != '_' && c != '?' && c != '`');
}

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







void find_target(t_token **head, t_token *target, t_token **temp, t_token **prev)
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

void delete_quotes_from_string(char *str)
{
	int in_single_quote;
	int in_double_quote;
	int j;
	int k;

	in_single_quote = 0;
	in_double_quote = 0;
	j = 0;
	k = 0;
	while (str[j])
	{
		if (str[j] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[j] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (str[j] == '$' && in_double_quote == 0 && in_single_quote == 0)
		{
			if (str[j] == '\'' && !in_double_quote)
				in_single_quote = !in_single_quote;
			else if (str[j] == '\"' && !in_single_quote)
				in_double_quote = !in_double_quote;
		}
		else
		{
			str[k] = str[j];
			k++;
		}
		j++;
	}
	str[k] = '\0';
}

void	delete_quotes_from_files(t_minishell *minishell)
{
	t_minishell			*temp;
	t_file_redirection	*files;

	temp = minishell;
	if (temp != NULL && temp->files != NULL)
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

void delete_quotes_from_args(t_minishell *minishell)
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

	delete_quotes_from_args(minishell);
	temp = minishell;
	delete_quotes_from_files(temp);
	return (minishell);
}

int	in_quote(const char *str)
{
	size_t	len;

	len = ft_strlen(str);
	return (len >= 2 && str[0] == '"' && str[len - 1] == '"');
}


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

int	handle_word_new_command(t_minishell_data_help *data, t_token *temp)
{
	char	*temp_args1;

	temp_args1 = ft_strjoin(data->args1, temp->value);
	free(data->args1);
	data->args1 = temp_args1;
	data->command = ft_strdup(temp->value);
	temp_args1 = ft_strjoin(data->args1, "\r");
	free(data->args1);
	data->args1 = temp_args1;
	free_args(data->args);
	data->args = ft_split2(data->args1, '\r');
	return (0);
}



void	token_to_minishell_helper(t_minishell_data_help *data, t_token **temp)
{
	if ((*temp)->type == T_WORD)
	{
		if (data->new_command == 1)
			data->new_command = handle_word_new_command(data, (*temp));
		else
			join_args((*temp), &data->args1, &data->args);
	}
	else if ((*temp)->type == T_REDIRECTION_IN)
		(*temp) = add_redirection_in((*temp), &data->files);
	else if ((*temp)->type == T_REDIRECTION_OUT)
		(*temp) = add_redirection_out((*temp), &data->files);
	else if ((*temp)->type == T_REDIRECTION_APPEND)
		(*temp) = add_redirection_append((*temp), &data->files);
	else if ((*temp)->type == T_HERDOC)
		(*temp) = add_heredoc((*temp), &data->files);
}

void	initialize_token_to_minishell_data(t_minishell_data_help *data)
{
	data->args1 = NULL;
	data->args = NULL;
	data->command = NULL;
	data->files = NULL;
	data->new_command = 1;
}


t_minishell	*token_to_minishell(t_token *tokens)
{
	t_minishell				*minishell;
	t_minishell_data_help	data;
	t_token					*temp;

	initialize_token_to_minishell_data(&data);
	minishell = NULL;
	temp = tokens;
	while (temp)
	{
		if (temp->type == T_PIPE)
		{
			add_minishell_back(&minishell,
				new_minishell(data.command, data.args, data.files));
			data.new_command = initialize_and_free(&data);
			temp = temp->next;
			continue ;
		}
		token_to_minishell_helper(&data, &temp);
		temp = temp->next;
	}
	add_minishell_back(&minishell,
		new_minishell(data.command, data.args, data.files));
	return (free(data.args1), minishell);
}

int	is_empty(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (is_whitespace(str[i]) == 0)
			return (0);
		i++;
	}
	return (1);

}






// end tokenization
void	handle_ctrl_c(int signal)
{
	if (signal == SIGINT)
	{
		// clear the current line	
		// rl_replace_line("", 0);
		// move to a new line
		printf("\n");
		//display the prmmpt on the new line
		rl_on_new_line();
		// redrws the readline line
		rl_redisplay();
		set_exit_status(*(lists_collecter->env), 130);
		return;
	}
}

void	handle_sigquit(int signal)
{
	if (signal == SIGQUIT)
	{
		// ignore signal
		return ;
	}
}

int	check_delemeter_for_sr(char *str, char *filename)
{
	if (str == NULL)
	{
		write(2, "bash: warning: here-document delimited by end-of-file\n",
			ft_strlen(\
				"bash: warning: here-document delimited by end-of-file\n"));
		return (1);
	}
	if (ft_strncmp(filename, str, ft_strlen(str) + 1) == 0)
	{
		free(str);
		return (1);
	}
	return (0);
}

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



// -------------------------------- start by exe one cmd -------------------------------------------
// --------------------------------------- end of exe one cmd ----------------------------------------------

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

int	help_heredoc_sr(int  *heredoc_counter, t_token *tokens, int error_code, t_file_redirection **heredocs)
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
			set_exit_status(*(lists_collecter->env), 2);
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
	set_exit_status(*(lists_collecter->env), 2);
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

void	print_heredocs(t_file_redirection *files)
{
	t_file_redirection	*temp;

	temp = files;
	while (temp)
	{
		printf("filename = %s [%s]\n",
			temp->filename, get_type_token(temp->type));
		temp = temp->next;
	}
}




//----------------------------------------- multiple commands --------------------------------------


void	collecter_init(t_minishell **minishell, t_environment **env, t_token **tokens)
{
	*env = NULL;
	*tokens = NULL;
	*minishell = NULL;
	lists_collecter = malloc(sizeof(t_lists_collecter));
	if (lists_collecter == NULL)
	{
		perror("malloc");
		exit(1);
	}
	lists_collecter->minishell = minishell;
	lists_collecter->env = env;
	lists_collecter->tokens = tokens;
	lists_collecter->p = 1;
}



int main(int argc, char **argv, char **base_env)
{
	t_environment	*env;
	t_token			*tokens;
	t_minishell		*minishell;
	char			*str;

	(void)argv;
	if (argc > 1)
		return (1);
	env = NULL;
	tokens = NULL;
	minishell = NULL;
	collecter_init(&minishell, &env, &tokens);
	get_environment(&env, base_env);
	printf("Welcome to minishell\n");
	while (1)
	{
		if (signal(SIGINT, handle_ctrl_c) == SIG_ERR)
		{
			perror("signal");
			return (1);
		}
		else if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		{
			perror("signal");
			return (1);
		}
		str = readline("$> ");
		if (str == NULL)
			break ;
		if (check_whitespaces(str) == 0)
		{
			free(str);
			continue ;
		}
		if (str[0] != '\0')
			add_history(str);
		// printf("before expand ======================\n");
		tokens = tokenize_input(str);
		// print_tokens(tokens);
		// printf("after expand ======================\n");
		tokens = expand(tokens);
		// print_tokens(tokens);
		// printf("after expand ======================\n");
		if (check_syntax_error(str) != 0 || tokens == NULL)
		{
			add_history(str);
			free(str);
			free_tokens(tokens);
			continue ;	
		}
		if (check_syntax_error_tokens(tokens) != 0 || tokens == NULL ||  ft_strlen(str) == 0 || ft_strncmp(str, ":", ft_strlen(str)) == 0)
		{
			t_file_redirection *heredocs;
			heredocs = NULL;
			int error_code = check_syntax_error_tokens(tokens);
			check_heredoc_for_syntax_error(&heredocs, tokens, error_code);
			free_heredocs(heredocs);
			add_history(str);
			free(str);
			free_tokens(tokens);
			continue;
		}
		minishell = token_to_minishell(tokens);
		// print_minishell(minishell);
		// printf("minishell after deleting quotes\n");
		minishell = delete_quotes(minishell);
		execution(minishell, &env);
		free(str);
		free_tokens(tokens);
		free_minishell(minishell);
	}
	free_environment(env);
	free(lists_collecter);
	return (0);
}