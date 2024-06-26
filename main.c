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

void	update_nb_quote(char c, int *nb_quote_single, int *nb_quote_double)
{
	if (c == '\'')
		*nb_quote_single += 1;
	if (c == '\"')
		*nb_quote_double += 1;
}

int	check_logical_operators(const char *str)
{
	int	i;
	int	in_single_quote;
	int	in_double_quote;

	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (str[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (str[i] == '&' && !in_single_quote && !in_double_quote)
		{
			while (str[i] && is_whitespace(str[i]))
				i++;
			if (str[i] == '&')
				return (puterr("Error: Logical Operators not supported.\n"), 1);
		}
		i++;
	}
	return (0);
}

void skip_whitespaces(const char *str, int *i)
{
	while (str[*i] && is_whitespace(str[*i]))
		(*i)++;
}

int	check_misplaced_operators_helper(const char *str, int *nb_quote_single
	, int *nb_quote_double,int *expect_operator)
{
	int	i;

	i = 0;
	while (str[i])
	{
		update_nb_quote(str[i], nb_quote_single, nb_quote_double);
		if (str[i] == '|' && *nb_quote_single % 2 == 0
			&& *nb_quote_double % 2 == 0)
		{
			if (*expect_operator == 1)
				return (ft_putstr_fd(\
					"Syntax error : Misplaced Operator\n", 2), 1);
			*expect_operator = 1;
		}
		else if (!is_whitespace(str[i]))
			*expect_operator = 0;
		i++;
	}
	return (0);
}

int	check_misplaced_operators(const char *str)
{
	int	i;
	int	nb_quote_single;
	int	nb_quote_double;
	int	expect_operator;

	i = 0;
	nb_quote_single = 0;
	nb_quote_double = 0;
	expect_operator = 0;
	skip_whitespaces(str, &i);
	if (str[i] == '|')
		return (ft_putstr_fd(\
			"Syntax error : Misplaced Operator at first of line\n", 2), 1);
	if (check_misplaced_operators_helper(str, &nb_quote_single
			, &nb_quote_double, &expect_operator) == 1)
		return (1);
	if (expect_operator == 1)
		return (ft_putstr_fd(\
			"Syntax error : Misplaced Operator at end of line\n", 2), 1);
	else
		return (0);
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
void	free_args(char **args)
{
	int	i;

	i = 0;
	if (args)
	{
		while (args[i])
		{
			free(args[i]);
			i++;
		}
		free(args);
	}
}

int	check_invalid_redirection(const char *str)
{
	int	i;
	int	nb_quote_single;
	int	nb_quote_double;

	i = 0;
	nb_quote_single = 0;
	nb_quote_double = 0;
	while (str[i])
	{
		update_nb_quote(str[i], &nb_quote_single, &nb_quote_double);
		if ((str[i] == '>' || str[i] == '<')
			&& nb_quote_single % 2 == 0 && nb_quote_double % 2 == 0)
		{
			if ((str[i] == '>' && str[i + 1] == '>')
				|| (str[i] == '<' && str[i + 1] == '<'))
				i++;
			if (str[i + 1] == '\0')
				return (printf("Syntax error : Invalid redirection\n"), 1);
			else if (str[i + 1] == '>' || str[i + 1] == '<')
				return (printf("Syntax error : Misplaced Operator\n"), 1);
		}
		i++;
	}
	return (0);
}

int	check_syntax_error(const char *str)
{
	if (check_unclosed_quotes(str) == 1)
		return (puterr("Syntax error : Unclosed quote\n"), 1);
	return (0);
}

// syntax error check
// tokenization

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

int	is_env_variable(char c)
{
	return (c == '$');
}

int	is_arithmetic_operator(char c)
{
	return (c == '+' || c == '-' || c == '*' || c == '/' || c == '=');
}
int	is_colone(char c)
{
	return (c == ':');
}
int	is_number(char c)
{
	return (c >= '0' && c <= '9');
}
int is_alpha(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}
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
int	s_minishell_size(t_minishell *minishell)
{
	int	size;

	size = 0;
	if (!minishell)
		return (0);
	while (minishell)
	{
		size++;
		minishell = minishell->next;
	}
	return (size);
}

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

t_file_redirection	*new_file_redirection(char *filename, t_type_of_token type)
{
	t_file_redirection	*file;

	file = (t_file_redirection *)malloc(sizeof(t_file_redirection));
	if (!file)
		return (NULL);
	file->filename = filename;
	file->should_expand_heredoc = 1;
	file->should_expand_heredoc = check_if_have_quotes(filename);
	file->type = type;
	file->next = NULL;
	return (file);
}

void	add_file_redirection_back(t_file_redirection **head
	, t_file_redirection *new_file)
{
	t_file_redirection	*temp;

	if (!*head)
		*head = new_file;
	else
	{
		temp = *head;
		while (temp->next)
			temp = temp->next;
		temp->next = new_file;
	}
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

void	handle_operator(t_token **head, const char *str, int *i)
{
	if (is_pipe(str[*i]))
		add_token_back(head, new_token(T_PIPE, ft_strdup("|")));
	else if (is_redirection_in(str[*i]))
	{
		if (str[*i + 1] == '<')
		{
			add_token_back(head, new_token(T_HERDOC, ft_strdup("<<")));
			(*i)++;
		}
		else
			add_token_back(head, new_token(T_REDIRECTION_IN, ft_strdup("<")));
	}
	else if (is_redirection_out(str[*i]))
	{
		if (str[*i + 1] == '>')
		{
			add_token_back(head, new_token(T_REDIRECTION_APPEND, \
				ft_strdup(">>")));
			(*i)++;
		}
		else
			add_token_back(head, new_token(T_REDIRECTION_OUT, ft_strdup(">")));
	}
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

void	handle_word(t_token **head, const char *str, int *i)
{
	int		in_quote;
	char	quote;
	char	*temp;
	int		j;

	in_quote = 0;
	j = *i;
	quote = '\0';
	while (str[*i])
	{
		update_quote_state(&in_quote, &quote, str[*i]);
		if (in_quote == 0)
		{
			if (is_operator(str[*i]) || is_whitespace(str[*i]))
				break ;
		}
		(*i)++;
	}
	temp = ft_substr(str, j, *i - j);
	add_token_back(head, new_token(T_WORD, temp));
	(*i)--;
}

t_token	*tokenize_input(const char *str)
{
	t_token	*head;
	int		i;

	head = NULL;
	i = 0;
	while (str[i])
	{
		while (str[i] && ft_strchr(" \t\n\v\f\r", str[i]))
			i++;
		if (str[i] == '\0')
			break ;
		if (str[i] && ft_strchr("<>|", str[i]))
			handle_operator(&head, str, &i);
		else
			handle_word(&head, str, &i);
		i++;
	}
	return (head);
}

void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current != NULL)
	{
		next = current->next;
		if (current->value != NULL)
		{
			free(current->value);
			current->value = NULL;
		}
		free(current);
		current = next;
	}
}
int check_syntax_error_tokens_helper2(t_token *temp)
{
	if (temp->type == T_HERDOC
		&& (temp->next == NULL
			|| temp->next->value[0] == '\0'
			|| temp->next->type != T_WORD))
	{
		if (temp->next == NULL)
			return (3);
		else if (temp->next->value[0] == '\0' || temp->next->type != T_WORD)
			return (9);
	}
	if ((temp->type == T_REDIRECTION_IN
			|| temp->type == T_REDIRECTION_OUT
			|| temp->type == T_REDIRECTION_APPEND
			|| temp->type == T_HERDOC)
		&& temp->next == NULL)
		return (1);
	return (0);
}

int	check_syntax_error_tokens_helper(t_token *temp)
{
	if (check_syntax_error_tokens_helper2(temp) != 0)
		return (check_syntax_error_tokens_helper2(temp));
	if (temp->type == T_PIPE && !temp->next)
		return (2);
	if (temp->type != T_WORD && temp->next->type == T_PIPE)
		return (2);
	if (temp->type == T_REDIRECTION_IN && temp->next->type != T_WORD)
		return (5);
	if (temp->type == T_HERDOC && temp->next->type != T_WORD)
		return (4);
	if (temp->type == T_REDIRECTION_OUT && temp->next->type != T_WORD)
		return (6);
	if (temp->type == T_REDIRECTION_APPEND && temp->next->type != T_WORD)
		return (7);
	if (temp->type == T_PIPE && temp->next->type == T_PIPE)
		return (8);
	return (0);
}

int	check_syntax_error_tokens(t_token *tokens)
{
	t_token	*temp;

	if (!tokens)
		return (0);
	temp = tokens;
	while (temp)
	{
		if (check_syntax_error_tokens_helper(temp) != 0)
			return (check_syntax_error_tokens_helper(temp));
		if (temp->prev == NULL && temp->type == T_PIPE)
			return (2);
		if (temp->type == T_PIPE && temp->next->type == T_REDIRECTION_IN)
			return (2);
		if (temp->type == T_PIPE && temp->next->type == T_REDIRECTION_OUT)
			return (2);
		if (temp->type == T_PIPE && temp->next->type == T_REDIRECTION_APPEND)
			return (2);
		temp = temp->next;
	}
	return (0);
}

t_minishell	*new_minishell(char *command, char **args, t_file_redirection *files)
{
	t_minishell	*minishell;

	minishell = (t_minishell *)malloc(sizeof(t_minishell));
	if (!minishell)
	{
		free(command);
		free(args);
		return (NULL);
	}
	minishell->heredoc_path = NULL;
	minishell->infile = 0;
	minishell->outfile = 0;
	minishell->pipe = 0;
	minishell->path = NULL;
	minishell->command = command;
	minishell->args = args;
	minishell->files = files;
	minishell->next = NULL;
	return (minishell);
}

void	add_minishell_back(t_minishell **head, t_minishell *new_minishell)
{
	t_minishell	*temp;

	if (!*head)
		*head = new_minishell;
	else
	{
		temp = *head;
		while (temp->next)
			temp = temp->next;
		temp->next = new_minishell;
	}
}

void	*new_arg(char *arg)
{
	t_args	*args;

	args = (t_args *)malloc(sizeof(t_args));
	if (!args)
		return (NULL);
	args->args = arg;
	args->next = NULL;
	return (args);
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

void	add_arg_back(t_args **head, t_args *new_arg)
{
	t_args	*temp;

	if (!*head)
		*head = new_arg;
	else
	{
		temp = *head;
		while (temp->next)
			temp = temp->next;
		temp->next = new_arg;
	}
}

static int	ft_count_words(char const *str, char sep)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && (str[i] == sep))
			i++;
		if (str[i] && !(str[i] == sep))
		{
			count++;
			while (str[i] && !(str[i] == sep))
				i++;
		}
	}
	return (count);
}

static char	**ft_free(char **strs)
{
	int	i;

	i = 0;
	while (strs[i])
		free(strs[i++]);
	free(strs);
	return (0);
}

void initialize_value_split2(int *j, char *quote, int *in_quote)
{
	*j = 0;
	*quote = '\0';
	*in_quote = 0;
}

void ft_skip2(int *i, int in_quote, const char *s, char c)
{
	if (in_quote == 0 && s[*i] == c)
		(*i)++;
}

char	**ft_split2_helper(char **res, char const *s, char c, int i)
{
	int		start;
	char	quote;
	int		in_quote;
	int		j;

	initialize_value_split2(&j, &quote, &in_quote);
	update_quote_state(&in_quote, &quote, s[i]);
	while (s[i])
	{
		update_quote_state(&in_quote, &quote, s[i]);
		ft_skip2(&i, in_quote, s, c);
		start = i;
		i--;
		while (s[++i] && (s[i] != c || in_quote == 1))
			update_quote_state(&in_quote, &quote, s[i]);
		if (i <= start)
			continue ;
		res[j] = (char *)malloc((i - start + 1) * sizeof(char));
		if (!res[j])
			return (ft_free(res));
		ft_strlcpy(res[j], &s[start], i - start + 1);
		j++;
	}
	res[j] = NULL;
	return (res);
}

char	**ft_split2(char const *s, char c)
{
	char	**res;
	int		i;

	i = 0;
	if (!s)
		return (NULL);
	res = (char **)malloc((ft_count_words(s, c) + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	res = ft_split2_helper(res, s, c , i);
	if (!res)
		return (ft_free(res));
	return (res);
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

char *change_value(char *str, int *i, int *index, char *env_variable)
{
	char			*new_value;
	t_environment	*get_env;
	char			*value;
	char			*first;
	char			*last;

	env_variable = ft_substr(str, *i + 1, *index - *i - 1);
	get_env = env_get_bykey(*(lists_collecter->env), env_variable);
	if (get_env == NULL)
		value = ft_strdup("");
	else
		value = ft_strdup(get_env->value);
	first = ft_substr(str, 0, *i);
	last = ft_strdup(&str[*index]);
	new_value = ft_strjoin(first, value);
	free(value);
	value = new_value;
	new_value = ft_strjoin(value, last);
	free(value);
	free(last);
	free(first);
	free(env_variable);
	free(str);
	return (new_value);
}

int expand_helper(int index, int *i, char *str)
{
	index = *i;
	index++;
	if (str[index] == '?' || is_number(str[index]))
		index++;
	else
	{
		while (str[index] && is_word(str[index]))
			index++;
	}
	return (index);
}

void init_expand_string(int *i, int *in_single_quotes, int *in_double_quotes, char **env_variable)
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

int count_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

void	*ambiguouse_redirect(char *old)
{
	if (old[0] != '\"' && old[ft_strlen(old) - 1] != '\"')
	{
		puterr("minishell: ");
		puterr(old);
		puterr(": ambiguous redirect\n");
		set_exit_status(*(lists_collecter->env), 1);
		return (NULL);
	}
	return ((void *)1);
}
void split_expanded_string(t_token *temp, t_token **tokens)
{
	char **args;
	int i;
	t_token *temp_to_remove;

	args = ft_split2(temp->value, ' ');
	i = 0;
	while(args[i])
	{
		insert_token(tokens, new_token(T_WORD, ft_strdup(args[i])), temp);
			i++;
	}
	free_args(args);
	temp_to_remove = temp;
	temp = temp->next;
	remove_token(tokens, temp_to_remove);
}

void expand_string_helper(t_token **tokens, t_token **temp)
{
	char *old;
	char *index_of_equal;
	char *index_of_dollar;

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
				ambiguouse_redirect(old);
		}
		split_expanded_string((*temp), tokens);
	}
	else
		(*temp) = (*temp)->next;
	free(old);
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
				if (ft_strchr(temp->value, '$') == NULL)
				{
					temp = temp->next;
					continue ;
				}
				expand_string_helper(&tokens, &temp);
			}
			else
				temp = temp->next;
		}
	}
	return (tokens);
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
void delete_quotes_from_files(t_minishell *minishell)
{
	t_minishell *temp;
	t_file_redirection *files;

	temp = minishell;
	if (temp!= NULL && temp->files != NULL)
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
	t_minishell *temp;
	int i;

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


t_token *add_redirection_in(t_token *temp, t_file_redirection **files)
{
	temp = temp->next;
	add_file_redirection_back(files, new_file_redirection(ft_strdup(temp->value)
			,T_REDIRECTION_IN));
	return (temp);
}

t_token *add_redirection_out(t_token *temp, t_file_redirection **files)
{
	temp = temp->next;
	add_file_redirection_back(files, new_file_redirection(ft_strdup(temp->value)
			,T_REDIRECTION_OUT));
	return (temp);
}

t_token *add_redirection_append(t_token *temp, t_file_redirection **files)
{
	temp = temp->next;
	add_file_redirection_back(files, new_file_redirection(ft_strdup(temp->value)
			,T_REDIRECTION_APPEND));
	return (temp);
}

t_token *add_heredoc(t_token *temp, t_file_redirection **files)
{
	temp = temp->next;
	add_file_redirection_back(files, new_file_redirection(ft_strdup(temp->value)
		,T_HERDOC));
	return (temp);
}

int initialize_and_free(t_minishell_data_help *data)
{
	data->command = NULL;
	free(data->args1);
	data->args = NULL;
	data->args1 = NULL;
	data->files = NULL;
	return (1);
}

int handle_word_new_command(t_minishell_data_help *data, t_token *temp)
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

void join_args(t_token *temp, char **args1, char ***args)
{
	char *temp_args1;

	temp_args1 = ft_strjoin(*args1, temp->value);
	free(*args1);
	*args1 = temp_args1;
	temp_args1 = ft_strjoin(*args1, "\r");
	free(*args1);
	*args1 = temp_args1;
	free_args(*args);
	*args = ft_split2(*args1, '\r');
}

void token_to_minishell_helper(t_minishell_data_help *data, t_token **temp)
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

void initialize_token_to_minishell_data(t_minishell_data_help *data)
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

void print_args(t_minishell *temp)
{
	int i;

	i = 0;
	if (temp->args)
	{
		printf("args: ");
		while (temp->args[i])
		{
			printf("[%s]", temp->args[i]);
			i++;
		}
	}
	printf("\n");
}

void print_files2(t_file_redirection *files)
{
	t_file_redirection *temp_files;

	temp_files = files;
	if (temp_files)
	{
		printf("files : \n");
		while(temp_files)
		{
			while (temp_files)
			{
				printf(" file: [%s] \n", temp_files->filename);
				printf("  type: %s \n", get_type_token(temp_files->type));
				temp_files = temp_files->next;
			}
			printf("\n");
		}
	}
}
void print_files(t_minishell *temp)
{
	t_file_redirection *temp_files;

	if (temp->files)
	{
		temp_files = temp->files;
		printf("files :\n");
		while (temp_files)
		{
			printf(" file: [%s] \n", temp_files->filename);
			printf("  type: %s \n", get_type_token(temp_files->type));
			temp_files = temp_files->next;
		}
		printf("\n");
	}

}

void	print_minishell(t_minishell *minishell)
{
	t_minishell			*temp;
	int					i;

	i = 0;
	temp = minishell;
	while (temp)
	{
		printf("--------------------\n");
		if (temp->command)
			printf("command [%d]: [%s]\n", i, temp->command);
		print_args(temp);
		print_files(temp);
		printf("--------------------\n");
		temp = temp->next;
		i++;
	}
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
int check_delemeter_for_sr(char *str, char *filename)
{
	if (str == NULL)
	{
		write(2, "bash: warning: here-document delimited by end-of-file\n",
			ft_strlen("bash: warning: here-document delimited by end-of-file\n"));
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
			break;
		if (str)
			free(str);
	}
}
void loop_heredoc_for_sr(t_file_redirection *heredocs)
{
	t_file_redirection	*temp;

	temp = heredocs;
	while (temp)
	{
		fill_heredoc_for_sr(temp);
		temp = temp->next;
	}
}

void	free_environment(t_environment *env)
{
	t_environment	*temp;

	if (!env)
		return ;
	while (env)
	{
		temp = env;
		env = env->next;
		free(temp->key);
		free(temp->value);
		free(temp);
	}
}

void free_files(t_file_redirection *files)
{
	t_file_redirection	*temp_files;
	t_file_redirection *temp_files_next;

	if (files)
	{
		temp_files = files;
		while (temp_files)
		{
			temp_files_next = temp_files->next;
			free(temp_files->filename);
			free(temp_files);
			temp_files = temp_files_next;
		}
	}
}
void	free_minishell(t_minishell *minishell)
{
	t_minishell			*temp;
	char				**temp_args;

	if (!minishell)
		return ;
	if (minishell->pipe)
		free(minishell->pipe);
	while (minishell)
	{
		temp = minishell;
		minishell = minishell->next;
		if (temp->command)
			free(temp->command);
		if (temp->heredoc_path)
			free(temp->heredoc_path);
		if (temp->path)
			free(temp->path);
		if (temp->args)
		{
			temp_args = temp->args;
			free_args(temp_args);
		}
		free_files(temp->files);
		free(temp);
	}
}

// -------------------------------- start by exe one cmd -------------------------------------------
// --------------------------------------- end of exe one cmd ----------------------------------------------

int check_heredoc_for_syntax_error(t_file_redirection **heredocs, t_token *tokens, int error_code)
{
	char *error_codes[10];
	int print_error = 1;
	// error_codes = malloc(sizeof(char *) * 9);
	error_codes[0] = "";
	error_codes[1] = "minishell: syntax error near unexpected token `newline'\n"; 
	error_codes[2] = "minishell: syntax error near unexpected token `|'\n"; 
	// error_codes[3] = "Syntax error: Expected a limiter after '<<'\n"; 
	error_codes[3] = "minishell: syntax error near unexpected token `newline'\n"; 
	error_codes[4] = "minishell: syntax error near unexpected token `<<'\n"; 
	error_codes[5] = "minishell: syntax error near unexpected token `<'\n"; 
	error_codes[6] = "minishell: syntax error near unexpected token `>'\n"; 
	error_codes[7] = "minishell: syntax error near unexpected token `>>'\n"; 
	error_codes[8] = "Error: Logical operators not supported YET.\n"; 
	error_codes[9] = "minishell: syntax error near unexpected token `<<'\n";
	// t_file_redirection *heredocs;
	t_token *temp;
	int heredoc_counter;

	temp = tokens;
	heredoc_counter = 0;
	if (error_code == 3 || error_code == 9)
	{
		return (printf("%s", error_codes[error_code]), 0);	
	}

	while(temp)
	{
		if (temp->type == T_HERDOC && temp->next != NULL)
		{
			add_file_redirection_back(heredocs, new_file_redirection(ft_strdup(temp->next->value), T_HERDOC));
			heredoc_counter++;
		}
		if (temp->type == T_PIPE)
		{
			set_exit_status(*(lists_collecter->env), 2);
			printf("%s", error_codes[error_code]);
			print_error = 0;
			if (heredoc_counter == 0)
				return (0);
			// return (0);
			// exit(2);
		}
		temp = temp->next;
		// ok then don't forget to complete that please ?!!!!!!!!!!!1
	}
	// print_heredocs(heredocs);
		// return (fork_heredoc(minishell, env));
	set_exit_status(*(lists_collecter->env), 2);
	if (heredoc_counter == 0)
		return (printf("%s", error_codes[error_code]), 0);
	if (heredoc_counter > 0 && heredoc_counter < 17)
	 {
		// printf("the heredoc should be openned here !\n");
		loop_heredoc_for_sr(*heredocs);
		if (print_error == 1)
			printf("%s", error_codes[error_code]);
		return (0);
	 }
	else if (heredoc_counter > 16)
	{
		write(2, "bash: maximum here-document count exceeded\n", 43);
		// free_lists_collector();
		return (0);
		// exit(2);
	}
	return (1);
}
void print_heredocs(t_file_redirection *files)
{
	t_file_redirection *temp;
	temp = files;
	while(temp)
	{
		printf("filename = %s [%s]\n", temp->filename, get_type_token(temp->type));
		temp = temp->next;
	}
}


void free_heredocs(t_file_redirection *files)
{
	t_file_redirection	*temp;

	while (files)
	{
		temp = files;
		files = files->next;
		free(temp->filename);
		free(temp);
	}
}

//----------------------------------------- multiple commands --------------------------------------


void collecter_init(t_minishell **minishell, t_environment **env, t_token **tokens)
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

void free_at_exit()
{
	if (*lists_collecter->env )
		free_environment(*lists_collecter->env);
	if (*lists_collecter->minishell )
		free_minishell(*lists_collecter->minishell);
	if (*lists_collecter->tokens )
		free_tokens(*lists_collecter->tokens);
	free(lists_collecter);
}

int main(int argc, char **argv, char **base_env)
{
	t_environment *env;
	t_token *tokens;
	t_minishell *minishell;
	char *str;
	
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
		if (signal(SIGINT, handle_ctrl_c) == SIG_ERR) {
			perror("signal");
			return 1;
		}
		else if (signal(SIGQUIT, SIG_IGN) == SIG_ERR) {
			perror("signal");
			return 1;
		}
		str = readline("$> ");
		if (str == NULL)
			break;
		if (check_whitespaces(str) == 0)
		{
			free(str);
			continue;
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
		if (check_syntax_error(str) != 0)
		{
		 	add_history(str);
			free(str);
			free_tokens(tokens);
			continue;	
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
		// if (minishell->command != NULL && is_empty(minishell->command) == 1)
		// {
		//     free(str);
		//     free_tokens(tokens);
		//     free_minishell(minishell);
		//     continue;
		// }
		// print_minishell(minishell);
		// printf("%s\n", str);
		execution(minishell, &env);
		free(str);
		free_tokens(tokens);
		free_minishell(minishell);
	}
	free_environment(env);
	free(lists_collecter);
	return (0);
}