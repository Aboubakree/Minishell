/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:18:34 by akrid             #+#    #+#             */
/*   Updated: 2024/06/10 17:39:42 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// syntax error checking/

t_lists_collecter *lists_collecter;

int count_char_occurence(char *str, int c)
{
    int i;
    int count;
    
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

int is_whitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

void update_nb_quote(char c, int *nb_quote_single, int *nb_quote_double)
{
    if (c == '\'')
        *nb_quote_single += 1;
    if (c == '\"')
        *nb_quote_double += 1;
}

int check_logical_operators(const char *str)
{
    int i;

    i = 0;
    if(!str)
        return (0);

    while(str[i] && is_whitespace(str[i]))
        i++;
    if (str[i] == '&')
        return (printf("Syntax error : Misplaced Operator at first of line\n"), 1);
    while (str[i] && str[i] != '&')
        i++;
    if (str[i] == '&')
    {
        if (str[i + 1] == '&')
            return (printf("Error: Logical operators '&&' and '||' are not supported YET.\n"), 1);
    }
    return (0);
}

int check_misplaced_operators(const char *str)
{
    int i;
    int nb_quote_single;
    int nb_quote_double;
    int expect_operator;
    
    i = 0;
    nb_quote_single = 0;
    nb_quote_double = 0;
    expect_operator = 0;
    while(str[i] && is_whitespace(str[i]))
        i++;
    if (str[i] == '|')
        return (printf("Syntax error : Misplaced Operator at first of line\n"), 1);
    i = 0;
    while(str[i])
    {
        update_nb_quote(str[i], &nb_quote_single, &nb_quote_double);
        if (str[i] == '|' && nb_quote_single % 2 == 0 && nb_quote_double % 2 == 0)
        {
            if (expect_operator == 1)
                return (printf("Syntax error : Misplaced Operator\n"), 1);
            expect_operator = 1;
        }
        else if (!is_whitespace(str[i]))
            expect_operator = 0;
        i++;
    }
    if (expect_operator == 1)
        return(printf("Syntax error : Misplaced Operator at end of line\n"), 1);
    else
        return (0);
}

int check_unclosed_quotes(const char *str)
{
    int i;
    int in_single_quotes;
    int in_double_quotes;

    i = 0;
    if (!str)
        return (0);
    in_single_quotes = 0;
    in_double_quotes = 0;
    while(str[i])
    {
        if (str[i] == '\'' && in_double_quotes == 0)
            in_single_quotes = !in_single_quotes;
        if (str[i] == '\"' && in_single_quotes == 0)
            in_double_quotes = !in_double_quotes;
        i++;
    }
    return (in_single_quotes || in_double_quotes);
}

int check_invalid_redirection(const char *str)
{
    int i;
    int nb_quote_single;
    int nb_quote_double;
    
    i = 0;
    nb_quote_single = 0;
    nb_quote_double = 0;
    while(str[i])
    {
        update_nb_quote(str[i], &nb_quote_single, &nb_quote_double);
        if ((str[i] == '>' || str[i] == '<') && nb_quote_single % 2 == 0 && nb_quote_double % 2 == 0)
        {
            if ((str[i] == '>' && str[i + 1] == '>') || (str[i] == '<' && str[i + 1] == '<'))
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

int check_syntax_error(const char *str, t_environment *env)
{
    if (check_unclosed_quotes(str) == 1)
        return (set_exit_status(env, 2), printf("Syntax error : Unclosed quote\n"), 1);
    if (check_misplaced_operators(str) == 1)
        return (set_exit_status(env, 2), 1);
    if (check_logical_operators(str) == 1)
        return (set_exit_status(env, 2), 1);
    if (check_invalid_redirection(str) == 1)
        return (set_exit_status(env, 2), 1);
    return (0);
}

// syntax error check
// tokenization

int is_operator(char c)
{
    return (c == '|' || c == '>' || c == '<');
}
int is_pipe(char c)
{
    return (c == '|');
}
int is_redirection_in(char c)
{
    return (c == '<');
}
int is_redirection_out(char c)
{
    return (c == '>');
}


int is_quote(char c)
{
    return (c == '\'' || c == '\"');
}


int is_env_variable(char c)
{
    return (c == '$');
}

int is_arithmetic_operator(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '=');
}
int is_colone(char c)
{
    return (c == ':');
}
int is_number(char c)
{
    return (c >= '0' && c <= '9');
}

int is_word(char c)
{
    return (!is_arithmetic_operator(c) 
        && !is_colone(c) 
        && !is_operator(c)
        && !is_quote(c)
        && !is_whitespace(c)
        && !is_env_variable(c)
        && c != '['
        && c != '%'
        && c != ']');
}
int s_minishell_size(t_minishell *minishell)
{
    int size;

    size = 0;
    if (!minishell)
        return (0);
    while(minishell)
    {
        size++;
        minishell = minishell->next;
    }
    return (size);
}


int tokens_size(t_token *tokens)
{
    int size;

    size = 0;
    if (!tokens)
        return (0);
    while(tokens)
    {
        size++;
        tokens = tokens->next;
    }
    return (size);
}
t_token get_last_token(t_token *tokens)
{
    t_token *temp;

    temp = tokens;
    if (!temp)
        return (t_token){0, NULL, NULL};
    while(temp->next)
        temp = temp->next;
    return (*temp);
}

int check_if_have_quotes(char *str)
{
    int i;
    int single_quotes;
    int double_quotes;
    single_quotes = 0;
    double_quotes = 0;

    i = 0;
    while(str[i])
    {
        if (str[i] == '\"')
            double_quotes++;
        else if (str[i] == '\'')
            single_quotes++;
        i++;
    }
    if (single_quotes >= 1 || double_quotes >= 1)
        return 0;
    return (1);
}

t_file_redirection *new_file_redirection(char *filename, t_type_of_token type)
{
    t_file_redirection *file;
   

    file = (t_file_redirection *)malloc(sizeof(t_file_redirection));
    if(!file)
        return (NULL);
    file->filename = filename;
    file->should_expand_heredoc = 1;
    file->should_expand_heredoc = check_if_have_quotes(filename);
    file->type = type;
    file->next = NULL;
    return (file);
}
void add_file_redirection_back(t_file_redirection **head, t_file_redirection *new_file)
{
    t_file_redirection *temp;

    if (!*head)
        *head = new_file;
    else
    {
        temp = *head;
        while(temp->next)
            temp = temp->next;
        temp->next = new_file;
    }
}

t_token *new_token(t_type_of_token type, char *value)
{
    t_token *token;

    token = (t_token *)malloc(sizeof(t_token));
    if(!token)
        return (NULL);
    token->type = type;
    token->value = value;
    token->next = NULL;
    return (token);
}

void add_token_front(t_token **head, t_token *new_token)
{
    new_token->next = *head;
    *head= new_token;
}
void add_token_back(t_token **head, t_token *new_token)
{
    t_token *temp;

    if (!*head)
        *head = new_token;
    else
    {
        temp = *head;
        while(temp->next)
            temp = temp->next;
        temp->next = new_token;
    }
}
char *get_type_token(t_type_of_token type)
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
void print_tokens(t_token *tokens)
{
    while(tokens)
    {
        printf("--------------------\n");
        // printf("type: %d, value: %s\n", tokens->type, tokens->value);
        printf("=======>type: %s, value: %s\n", get_type_token(tokens->type), tokens->value);
        printf("--------------------\n");
        tokens = tokens->next;
    }
}

void handle_operator(t_token **head, const char *str, int *i)
{
    if (is_pipe(str[*i]))
        add_token_back(head, new_token(T_PIPE, ft_strdup("|")));
    else if (is_redirection_in(str[*i]))
    {
        if (str[*i + 1] == '<')
        {
            add_token_back(head, new_token(T_HERDOC, ft_strdup("<<")));
            (*i) ++;
        }
        else
            add_token_back(head, new_token(T_REDIRECTION_IN, ft_strdup("<")));
    }
    else if (is_redirection_out(str[*i]))
    {
        if (str[*i + 1] == '>')
        {
            add_token_back(head, new_token(T_REDIRECTION_APPEND, ft_strdup(">>")));
            (*i) ++;
        }
        else
            add_token_back(head, new_token(T_REDIRECTION_OUT, ft_strdup(">")));
    }
}

void    update_quote_state(int *in_quote, char *quote, char c)
{
    if (*in_quote == 0 && (c == '\'' || c == '\"'))
    {
        *in_quote = 1;
        *quote = c;
    }
    else if (*in_quote == 1 && c == *quote)
        *in_quote = 0;
}

void    handle_word(t_token **head, const char *str, int *i)
{
    int in_quote;
    char quote;
    char *temp;

    in_quote = 0;
    int j = *i;
    quote = '\0';
    while(str[*i])
    {
        update_quote_state(&in_quote, &quote, str[*i]);
        if (in_quote == 0)
        {
            if (is_operator(str[*i]) || is_whitespace(str[*i]))
                break;
        }
        (*i)++;
    }
    temp = ft_substr(str, j, *i - j);
    add_token_back(head, new_token(T_WORD, temp));
    (*i)--;
}

t_token *tokenize_input(const char *str)
{
    t_token *head;
    int i;

    head = NULL;
    i = 0;
    while(str[i])
    {
        while(str[i]  &&  ft_strchr(" \t\n\v\f\r", str[i]))
            i++;
        if (str[i] && ft_strchr("<>|", str[i]))
            handle_operator(&head, str, &i);
        else
            handle_word(&head, str, &i);
        i++;
    }
    return (head);
}

void free_tokens(t_token *tokens)
{
    t_token *current, *next;

    current = tokens;
    while(current != NULL)
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
// void free_tokens(t_token *tokens)
// {
//     t_token *temp;

//     if (!tokens)
//         return ;
//     while(tokens)
//     {
//         temp = tokens;
//         tokens = tokens->next;
//         free(temp->value);
//         free(temp);
//     }
// }

int check_syntax_error_tokens(t_token *tokens)
{
    t_token *temp;

    if (!tokens)
        return (0);
    temp = tokens;
    while(temp)
    {
        if (temp->type == T_HERDOC && temp->next->type != T_WORD)
            return (printf("Syntax error : Expected a limiter after '<<'\n"), 1);
        if (temp->type == T_REDIRECTION_IN && temp->next->type != T_WORD)
            return (printf("Syntax error : Expected a file after '<'\n"), 1);
        if (temp->type == T_REDIRECTION_OUT && temp->next->type != T_WORD)
            return (printf("Syntax error : Expected a file after '>'\n"), 1);
        if (temp->type == T_REDIRECTION_APPEND && temp->next->type != T_WORD)
            return (printf("Syntax error : Expected a file after '>>'\n"), 1);
        if (temp->type == T_PIPE && !temp->next)
            return (printf("Syntax error : Expected a command after '|'\n"), 1);
        if (temp->type == T_PIPE && temp->next->type == T_PIPE)
            return (printf("Error: Logical operators '&&' and '||' are not supported YET.\n"), 1);
        if (temp->type == T_PIPE && temp->next->type == T_REDIRECTION_IN)
            return (printf("Syntax error : Expected a command after '|'\n"), 1);
        if (temp->type == T_PIPE && temp->next->type == T_REDIRECTION_OUT)
            return (printf("Syntax error : Expected a command after '|'\n"), 1);
        if (temp->type == T_PIPE && temp->next->type == T_REDIRECTION_APPEND)
            return (printf("Syntax error : Expected a command after '|'\n"), 1);
       
        temp = temp->next;
    }
    return (0);
}
t_minishell *new_minishell(char *command, char **args, t_file_redirection *files)
{
    t_minishell *minishell;

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
void add_minishell_back(t_minishell **head, t_minishell *new_minishell)
{
    t_minishell *temp;

    if (!*head)
        *head = new_minishell;
    else
    {
        temp = *head;
        while(temp->next)
            temp = temp->next;
        temp->next = new_minishell;
    }
}


void *new_arg(char *arg)
{
    t_args *args;

    args = (t_args *)malloc(sizeof(t_args));
    if (!args)
        return (NULL);
    args->args = arg;
    args->next = NULL;
    return (args);
}

int check_whitespaces(const char *str)
{
    int i;

    i = 0;
    while(str[i])
    {
        if (is_whitespace(str[i]) != 1)
            return (1);
        i++;
    }
    return (0);
}
void add_arg_back(t_args **head, t_args *new_arg)
{
    t_args *temp;

    if (!*head)
        *head = new_arg;
    else
    {
        temp = *head;
        while(temp->next)
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
static char	**do_it2(char **res, char const *s, char c, int i)
{
	int	start;
	int	end;
    char quote;
    int in_quote;
	int	j;

	j = 0;
    quote = '\0';
    in_quote = 0;
    update_quote_state(&in_quote, &quote, s[i]);
	while (s[i])
	{
        update_quote_state(&in_quote, &quote, s[i]);
        if (in_quote == 0 && s[i] == c)
            i++;
        start = i;
        while (s[i] && (s[i] != c || in_quote == 1))
        {
            update_quote_state(&in_quote, &quote, s[i]);
            i++;
        }
        end = i;
        if (end > start)
        {
            res[j] = (char *)malloc((end - start + 1) * sizeof(char));
            if (!res[j])
                return (ft_free(res));
            ft_strlcpy(res[j], &s[start], end - start + 1);
            j++;
        }
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
    res = do_it2(res, s, c, i);
    if (!res)
        return (ft_free(res));
    return (res);
}

char *expand_string(char *str, t_environment *env, int from_heredoc)
{
    int i = 0;
    char *value;
    char *first;
    char *new_value;
    char *last;
    int in_single_quotes = 0;
    int in_double_quotes = 0;
    while(str[i])
    {
        if (str[i] == '\'' && in_single_quotes == 0 && in_double_quotes == 0)
            in_single_quotes = 1;
        else if (str[i] == '\'' && in_single_quotes == 1)
            in_single_quotes = 0;
        else if (str[i] == '\"' && in_double_quotes == 0 && in_single_quotes == 0)
            in_double_quotes = 1;
        else if (str[i] == '\"' && in_double_quotes == 1)
                 in_double_quotes = 0;
        if ((str[i] == '$' && in_single_quotes == 0 && from_heredoc == 0 && is_word(str[i + 1])) || (str[i] == '$' && from_heredoc == 1))
        {
            if (is_whitespace(str[i + 1]) || str[i + 1] == '\0')
            {
                i++;
                continue;
            }
            int index = i;
            index++;
            if (str[index] == '?' || is_number(str[index]))
                index++;
            else
            {
                while(str[index] && is_word(str[index]))
                    index++;
            }
            char *env_variable = ft_substr(str, i + 1, index - i - 1);
            // printf("env_variable: %s\n", env_variable);
            t_environment *get_env = env_get_bykey(env, env_variable);
            if(get_env == NULL)
            {
                value = ft_strdup("");
            }
            else
                value = ft_strdup(get_env->value);
            first = ft_substr(str, 0, i);
            last = ft_strdup(&str[index]);
            new_value = ft_strjoin(first, value);
            free(value);
            char *temp2 = new_value;
            new_value = ft_strjoin(temp2, last);
            free(temp2);
            free(last);
            free(first);
            free(env_variable);
            free(str);
            str = new_value;
        }
        else
            i++;
    }
    if (str[0] == '$' && (str[1] == '\'' || str[1] == '\"'))
    {
        i = 0;
        while(str[i])
        {
            str[i] = str[i + 1];
            i++;
        }
    }
    return (str);
}

t_token *expand(t_token *tokens, t_environment *env)
{
    t_token *temp = tokens;
    while(temp)
    {
        if (temp->type == T_WORD)
        {
            if (ft_strchr(temp->value, '$') == NULL)
            {
                temp = temp->next;
                continue;
            }
            temp->value  = expand_string(temp->value, env, 0);
        }
        temp = temp->next;
    }
    return (tokens);
}

int ft_remove_char(char *str, int index)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while(str[i])
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

t_minishell *delete_quotes(t_minishell *minishell)
{
    t_minishell *temp;
    int in_single_quote, in_double_quote;
    int i, j, k;

    temp = minishell;
    if (temp->args != NULL)
    {
        while (temp)
        {
            i = 0;
            while (temp->args[i])
            {
                in_single_quote = 0;
                in_double_quote = 0;
                j = 0;
                k = 0;
                while (temp->args[i][j])
                {
                    if (temp->args[i][j] == '\'' && !in_double_quote)
                        in_single_quote = !in_single_quote;
                    else if (temp->args[i][j] == '\"' && !in_single_quote)
                        in_double_quote = !in_double_quote;
                    else
                    {
                        temp->args[i][k] = temp->args[i][j];
                        k++;
                    }
                    j++;
                }
                temp->args[i][k] = '\0';
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
    temp = minishell;
    }
    if(temp!= NULL && temp->files != NULL)
    {
        t_file_redirection *temp_files;
        temp = minishell;
        while(temp)
        {
            temp_files = temp->files;
            while(temp_files)
            {
                in_single_quote = 0;
                in_double_quote = 0;
                j = 0;
                k = 0;
                while(temp_files->filename[j])
                {
                    if (temp_files->filename[j] == '\'' && !in_double_quote)
                        in_single_quote = !in_single_quote;
                    else if (temp_files->filename[j] == '\"' && !in_single_quote)
                        in_double_quote = !in_double_quote;
                    else
                    {
                        temp_files->filename[k] = temp_files->filename[j];
                        k++;
                    }
                    j++;
                }
                temp_files->filename[k] = '\0';
                temp_files = temp_files->next;
            }
            temp = temp->next;
        }
    }
    return (minishell);
}
void free_args(char **args)
{
    int i = 0;
    if (args)
    {
        while(args[i])
        {
            free(args[i]);
            i++;
        }
        free(args);
    }
}
int in_quote(const char *str)
{
    size_t len;

    len = ft_strlen(str);
    return (len >= 2 && str[0] == '"' && str[len - 1] == '"');
}

int check_if_have_space(const char *str)
{
    int i;

    i = 0;
    while(str[i])
    {
        if (is_whitespace(str[i]) == 1)
            return (1);
        i++;
    }
    return (0);
}
t_minishell *token_to_minishell(t_token *tokens)
{
    t_minishell *minishell;
    char *command = NULL;
    char *args1 = NULL;
    char **args = NULL;
    char *temp_args1;
    t_file_redirection *files = NULL;
    int new_command = 1;

    minishell = NULL;
    t_token *temp = tokens;
    while(temp)
    {
        if (temp->type == T_PIPE)
        {
            add_minishell_back(&minishell, new_minishell(command, args, files));
            command = NULL;
            free(args1);
            args = NULL;
            args1 = NULL;
            files = NULL;
            temp = temp->next;
            new_command = 1;
            continue;
        }
        if (temp->type == T_WORD)
        {
            // printf("temp->value = [[%s]]\n", temp->value);
            if (new_command == 1)
            {
                temp_args1 = ft_strjoin(args1, temp->value);
                free(args1);
                args1 = temp_args1;
                command = ft_strdup(temp->value);
                temp_args1 = ft_strjoin(args1, "\r");
                free(args1);
                args1 = temp_args1;
                free_args(args);
                args = ft_split2(args1, '\r');
                new_command = 0;
            }
            else
            {
                temp_args1 = ft_strjoin(args1, temp->value);
                free(args1);
                args1 = temp_args1;
                temp_args1 = ft_strjoin(args1, "\r");
                free(args1);
                args1 = temp_args1;
                free_args(args);
                args = ft_split2(args1, '\r');
            }
        }
        else if (temp->type == T_REDIRECTION_IN)
        {
            temp = temp->next;
            if (check_if_have_space(temp->value) == 1 && in_quote(temp->value) == 0)
                printf("%sbash: [%s]: ambiguous redirect\n%s",RED,  temp->value, RESET);
            add_file_redirection_back(&files, new_file_redirection(ft_strdup(temp->value), T_REDIRECTION_IN));
        }
        else if (temp->type == T_REDIRECTION_OUT)
        {
            temp = temp->next;
            if (check_if_have_space(temp->value) == 1 && in_quote(temp->value) == 0)
                printf("%sbash: [%s]: ambiguous redirect\n%s",RED,  temp->value, RESET);
            else
                add_file_redirection_back(&files, new_file_redirection(ft_strdup(temp->value), T_REDIRECTION_OUT));
        }
        else if (temp->type == T_REDIRECTION_APPEND)
        {
            temp = temp->next;
            if (check_if_have_space(temp->value) == 1 && in_quote(temp->value) == 0)
                printf("%sbash: [%s]: ambiguous redirect\n%s",RED,  temp->value, RESET);
            else
                add_file_redirection_back(&files, new_file_redirection(ft_strdup(temp->value), T_REDIRECTION_APPEND));
        }
        else if (temp->type == T_HERDOC)
        {
            temp = temp->next;
            add_file_redirection_back(&files, new_file_redirection(ft_strdup(temp->value), T_HERDOC));
        }
        temp = temp->next;
    }
    add_minishell_back(&minishell, new_minishell(command, args, files));
    free(args1);
    return (minishell);
}
int is_empty(char *str)
{
    int i;

    i = 0;
    while(str[i])
    {
        if (is_whitespace(str[i]) == 0)
            return (0);
        i++;
    }
    return (1);

}
void print_minishell(t_minishell *minishell)
{
    t_minishell *temp;
    temp = minishell;
    int i = 0;
    while(temp)
    {
        printf("--------------------\n");
        if (temp->command)
            printf("command [%d]: [%s] ====> empty : %d\n",i,  temp->command, is_empty(temp->command));
        if (temp->args)
        {
            printf("args: ");
        int j = 0;
        if (temp->args)
        {
            while(temp->args[j])
            {
                printf("[%s]", temp->args[j]);
                j++;
            }
        }
        printf("\n");
        }
        if (temp->files)
        {
            t_file_redirection *temp_files = temp->files;
            printf("files :\n");
            while(temp_files)
            {
                printf(" file: [%s] \n", temp_files->filename);
                printf("  type: %s \n", get_type_token(temp_files->type));
                temp_files = temp_files->next;
            }
            printf("\n");
        }
        printf("--------------------\n");
        temp = temp->next;
        i++;
    }
}
// end tokenization
#include <readline/readline.h>
#include <readline/history.h>
void handle_ctrl_c(int signal)
{
    if (signal == SIGINT)
    {
        // clear the current line
        rl_replace_line("", 0);
        // move to a new line
        printf("\n");
        //display the prmmpt on the new line
        rl_on_new_line();
        // redrws the readline line
        rl_redisplay();
        return;
    }
}
void handle_sigquit(int signal)
{
    if (signal == SIGQUIT)
    {
        // ignore signal
        return;
    }
}


void free_environment(t_environment *env)
{
    t_environment *temp;

    if (!env)
        return ;
    while(env)
    {
        temp = env;
        env = env->next;
        free(temp->key);
        free(temp->value);
        free(temp);
    }
}
void free_minishell(t_minishell *minishell)
{
    t_minishell *temp;
    char **temp_args;
    t_file_redirection *temp_files, *temp_files_next;

    if (!minishell)
        return;
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
        if (temp->files)
        {
            temp_files = temp->files;
            while (temp_files)
            {
                temp_files_next = temp_files->next;
                free(temp_files->filename);
                free(temp_files);
                temp_files = temp_files_next;
            }
        }
        free(temp);
    }
}

// -------------------------------- start by exe one cmd -------------------------------------------

int args_count(char **args)
{
    int i;

    i = 0;
    while (args && args[i])
        i ++;
    return (i);
}

int    check_builtin(t_minishell *singl_mini, t_environment **env)
{
    if (singl_mini->command == NULL)
        return (1);
    if (ft_strncmp("cd", singl_mini->command, 3) == 0)
        return (cd(singl_mini, *env), 0);
    if (ft_strncmp("echo", singl_mini->command, 5) == 0)
       return (echo(singl_mini, *env), 0);
    if (ft_strncmp("env", singl_mini->command, 4) == 0)
        return (envi(singl_mini, *env), 0);
    if (ft_strncmp("pwd", singl_mini->command, 4) == 0 )
        return (pwd(singl_mini, *env), 0);
    if (ft_strncmp("export", singl_mini->command, 7) == 0)
        return (export(singl_mini, env), 0);
    if (ft_strncmp("unset", singl_mini->command, 6) == 0)
        return (unset(singl_mini, env), 0);
    if (strncmp("exit", singl_mini->command, 5) == 0)
        return (fake_exit(singl_mini, *env), 0);
    return (1);
}

int cmd_count(t_minishell *minishell)
{
    int count;

    count = 0;
    while (minishell)
    {
        count ++;
        minishell = minishell->next;
    }
    return (count);
}


void handle_heredoc_signals(int signal)
{
    if (signal == SIGINT)
    {
        // Clear the current line and move to a new line
        rl_replace_line("", 0);
        printf("\n");
        rl_on_new_line();
        // rl_redisplay();
        free_at_exit();
        exit(130);
    }
}


int check_delemeter(char *str, char *filename, int fd)
{
    if (str == NULL)
    {
        write(2, "bash: warning: here-document delimited by end-of-file\n",
            ft_strlen("bash: warning: here-document delimited by end-of-file\n"));
        close(fd);
        return (1);
    }
    if (ft_strncmp(filename, str, ft_strlen(str) + 1) == 0)
    {
        free(str);
        close(fd);
        return (1);
    }
    return (0);
}


void fill_heredoc(t_minishell *temp, t_file_redirection *files, t_environment *env)
{
    int     fd;
    char    *str;

    fd = open(temp->heredoc_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
        perror("open");
    while (1)
    {
        signal(SIGINT, handle_heredoc_signals);
        str = readline(">");
        if (check_delemeter(str, files->filename, fd) == 1)
            break;
        if (files->should_expand_heredoc == 1 && ft_strchr(str, '$') != NULL)
            str = expand_string(str, env, 1);
        write(fd, str, ft_strlen(str));
        write(fd, "\n", 1);
        if(str)
            free(str);
    }
    close(fd);
}

void loop_heredoc(t_minishell *minishell, t_environment *env)
{
    t_file_redirection *files;
    t_minishell *temp;

    temp = minishell;
    while (temp)
    {
        files = temp->files;
        while (files)
        {
            if (files->type == T_HERDOC)
                fill_heredoc(temp, files, env);
            files = files->next;
        }
        temp = temp->next;
    }
    free_at_exit();
    exit(0);
}

int  fork_heredoc(t_minishell *minishell , t_environment *env)
{
    pid_t   pid;
    int     status;

    status = 0;
    pid = fork();
    if (pid == 0)
        loop_heredoc(minishell, env);
    wait(&status);
    status = status >> 8;
    set_exit_status(env, status);
    return (status);
}

int check_heredoc(t_minishell *minishell , t_environment *env, int i)
{
    t_file_redirection *files;
    t_minishell *temp;

    temp = minishell;
    while (temp)
    {
        files = minishell->files;
        while (files)
        {
            if (files->type == T_HERDOC)
                i ++;
            files = files->next;
        }
        temp = temp->next;
    }
    if (i > 0 && i < 17)
        return (fork_heredoc(minishell, env));
    else if (i > 16)
    {
        write(2, "bash: maximum here-document count exceeded\n", 43);
        free_at_exit();
        exit(2);
    }
    return (0);
}

int is_builtin(char *cmd)
{
    if (cmd == NULL)
        return (0);
    if (ft_strncmp("cd", cmd, 3) == 0)
        return (1);
    if (ft_strncmp("echo", cmd, 5) == 0)
       return (1);
    if (ft_strncmp("env", cmd, 4) == 0)
        return (1);
    if (ft_strncmp("pwd", cmd, 4) == 0 )
        return (1);
    if (ft_strncmp("export", cmd, 7) == 0)
        return (1);
    if (ft_strncmp("unset", cmd, 6) == 0)
        return (1);
    if (strncmp("exit", cmd, 5) == 0)
        return (1);
    return (0);
}

int file_error(t_minishell *minishell, t_environment *env, char *filename)
{
    write(2, "bash: ",6);
    write(2, filename, ft_strlen(filename));
    write(2, ": ", 2);
    perror("");
    if (minishell->nbr_cmd == 1 && is_builtin(minishell->command))
    {
        set_exit_status(env, 1);
        return (1);
    }
    set_exit_status(env, 1);
    free_at_exit();
    exit(1);
}


int open_file_extended(t_minishell *minishell, t_environment *env, t_file_redirection *files)
{
    if (files->type == T_REDIRECTION_APPEND)
    {
        if (minishell->outfile != 1)
            close(minishell->outfile);
        minishell->outfile = open(files->filename, O_WRONLY | O_APPEND | O_CREAT, 0664);
        if (minishell->outfile < 0)
            return (file_error(minishell, env, files->filename));
    }
    else if (files->type == T_HERDOC)
    {
        if (minishell->infile != 0)
            close(minishell->infile);
        minishell->infile = open(minishell->heredoc_path, O_RDONLY, 0644);
        if (minishell->infile < 0)
            return (file_error(minishell, env, minishell->heredoc_path));
    }
    return (0);
}

int open_files(t_minishell *minishell, t_environment *env, t_file_redirection *files)
{
    files = minishell->files;
    while (files)
    {
        if (files->type == T_REDIRECTION_IN)
        {
            if (minishell->infile != 0)
                close(minishell->infile);
            minishell->infile = open(files->filename, O_RDONLY, 0644);
            if (minishell->infile < 0)
                return (file_error(minishell, env, files->filename));
        }
        else if (files->type == T_REDIRECTION_OUT)
        {
            if (minishell->outfile != 1)
                close(minishell->outfile);
            minishell->outfile = open(files->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (minishell->outfile < 0)
                return (file_error(minishell, env, files->filename));
        }
        else if (open_file_extended(minishell, env, files))
            return (1);
        files = files->next;
    }
    return (0);
}


void    start_execute_one(t_minishell *minishell, t_environment **env)
{
    char **env_conv;

    open_files(minishell, *env, minishell->files);
    minishell->path = get_cmd_path(minishell->command, *env, 0);
    if (minishell->path == NULL)
    {
        free_at_exit();
        exit(127);
    }
    dup2(minishell->infile, 0);
    dup2(minishell->outfile, 1);
    env_conv = convert_env(*env);
    execve(minishell->path, minishell->args, env_conv);
    perror("bash");
    free_split(env_conv);
    free_at_exit();
    exit(126);
}

void    execute_one(t_minishell *minishell, t_environment **env)
{
    pid_t   pid;
    int     status;

    pid = fork();
    if (pid == 0)
        start_execute_one(minishell, env);
    wait(&status);
    unlink_files(minishell);
    status = status >> 8;
    set_exit_status(*env, status);
}


// --------------------------------------- end of exe one cmd ----------------------------------------------


//----------------------------------------- multiple commands --------------------------------------


void    pipe_init(t_minishell *mini)
{
    int i;
    int *pip;
    t_minishell *temp;

    i = 0;
    pip = malloc(sizeof(int) * 2 * (mini->nbr_cmd - 1));
    while (i < mini->nbr_cmd - 1)
    {
        if (pipe(pip + i * 2))
        {
            perror("pipe");
            free_at_exit();
            exit(1);
        }
        i ++;
    }
    temp = mini;
    while (temp )
    {
        temp->pipe = pip;
        temp = temp->next;
    }
}

void close_pipes(int *pipe, int nbr_cmd)
{
    int i;

    i = 0;
    while (i < 2 * (nbr_cmd - 1))
        close(pipe[i ++]);
}

void unlink_files(t_minishell *minishell)
{
    while (minishell)
    {
        if (access(minishell->heredoc_path, F_OK) == 0)
        {
            if (unlink(minishell->heredoc_path))
                perror("unlink");
        }
        minishell = minishell->next;
    }
}

void wait_childs(t_minishell *mini, t_environment *env, int num_cmd)
{
    int i;
    int status;

    i = 0;
    close_pipes(mini->pipe, mini->nbr_cmd);
    while (i < num_cmd)
    {
        if (wait(&status) > 0)
        {
            status = status >> 8;
            set_exit_status(env, status);
            i ++;
        }
        else
        {
            perror("wait");
            i ++;
        }
    }
    unlink_files(mini);
}

void get_in_out_extended(t_minishell *singl_mini)
{
    if (singl_mini->infile == 0)
        dup2(singl_mini->pipe[(singl_mini->cmd_order - 1) * 2], 0);
    else
        dup2(singl_mini->infile, 0);
    if (singl_mini->outfile == 1)
        dup2(singl_mini->pipe[singl_mini->cmd_order * 2 + 1], 1);
    else
        dup2(singl_mini->outfile, 1);
}

void get_in_out_priorities(t_minishell *singl_mini)
{
    if (singl_mini->cmd_order == 0)
    {
        dup2(singl_mini->infile, STDIN_FILENO);
        if (singl_mini->outfile == STDOUT_FILENO)
            dup2(singl_mini->pipe[1], STDOUT_FILENO);
        else
            dup2(singl_mini->outfile, 1);
    }
    else if (singl_mini->cmd_order == singl_mini->nbr_cmd - 1)
    {
        if (singl_mini->infile == 0)
            dup2(singl_mini->pipe[(singl_mini->cmd_order - 1) * 2], 0);
        else
            dup2(singl_mini->infile, 0);
        dup2(singl_mini->outfile, 1);
    }
    else
        get_in_out_extended(singl_mini);
    close_pipes(singl_mini->pipe, singl_mini->nbr_cmd);
}


void final_execution(t_minishell *singl_mini, t_environment **env)
{
    char **env_conv;

    open_files(singl_mini, *env, singl_mini->files);
    get_in_out_priorities(singl_mini);
    if (check_builtin(singl_mini, env) == 0)
    {
        free_at_exit();
        exit (0);
    }
    singl_mini->path = get_cmd_path(singl_mini->command, *env, 0);
    if (singl_mini->path == NULL)
    {
        free_at_exit();
        exit(127);
    }
    env_conv = convert_env(*env);
    execve(singl_mini->path, singl_mini->args, env_conv );
    perror("bash");
    free_split(env_conv);
    free_at_exit();
    exit(126);
}

void execute_all(t_minishell *minishell, t_environment **env)
{
    pid_t       pid;
    t_minishell *temp;
    
    pipe_init(minishell);
    temp = minishell;
    while (temp)
    {
        pid = fork();
        if (pid == 0)
            final_execution(temp, env);
        temp = temp->next;
    }
    wait_childs(minishell, *env, minishell->nbr_cmd);
}


//----------------------------------------- end of multiple command --------------------------------
void    restore_STD_IN_OUT(int stdout, int stdin)
{
    dup2(stdin, STDIN_FILENO);
    dup2(stdout, STDOUT_FILENO);
    close(stdin);
    close(stdout);
}

char    *get_herdoc_path(int i)
{
    char *heredoc_path;

    heredoc_path = malloc(sizeof(char) * (ft_strlen("/tmp/herdoc_buffer_") + 2));
    ft_strlcpy(heredoc_path, "/tmp/herdoc_buffer_", 19);
    heredoc_path[ft_strlen("/tmp/herdoc_buffer_")] = i + 'a';
    heredoc_path[ft_strlen("/tmp/herdoc_buffer_") + 1] = '\0';
    return (heredoc_path);
}

void    minishell_init(t_minishell *minishell, int count_cmds)
{
    int i;
    t_minishell *temp;

    i = 0;
    temp = minishell;
    while(temp)
    {
        temp->cmd_order = i;
        temp->path = NULL;
        temp->pipe = NULL;
        temp->infile = 0;
        temp->outfile = 1;
        temp->nbr_cmd = count_cmds;
        temp->heredoc_path = get_herdoc_path(i);
        i ++;
        temp = temp->next;
    }
}

void set_under_score(t_minishell *minishell, t_environment **env)
{
    t_environment *env_under_score;

    env_under_score = env_get_bykey(*env, "_");
    if (env_under_score)
    {
        if (args_count(minishell->args) > 0)
        {
            free(env_under_score->value);
            env_under_score->value = ft_strdup(minishell->args[args_count(minishell->args) - 1]);
        }
        else
        {
            free(env_under_score->value);
            env_under_score->value = ft_strdup("");
        }
    }
}

void    execution(t_minishell *minishell, t_environment **env)
{
    int stdout;
    int stdin;

    stdout = dup(STDOUT_FILENO);
    stdin = dup(STDIN_FILENO);
    minishell_init(minishell, cmd_count(minishell));
    if (check_heredoc(minishell, *env, 0))
        return;
    if (cmd_count(minishell) == 1)
    {
        if (check_builtin(minishell, env) == 0)
        {
            set_under_score(minishell, env);
            restore_STD_IN_OUT(stdout, stdin);
            return ;
        }
        else
            execute_one(minishell, env);
        set_under_score(minishell, env);
    }
    else
        execute_all(minishell, env);
    restore_STD_IN_OUT(stdout, stdin);
}

void collecter_init(t_minishell **minishell, t_environment **env, t_token **tokens)
{
    lists_collecter = malloc(sizeof(t_lists_collecter));
    if (lists_collecter == NULL)
    {
        perror("malloc");
        exit(1);
    }
    *env = NULL;
    *tokens = NULL;
    *minishell = NULL;
    lists_collecter->minishell = minishell;
    lists_collecter->env = env;
    lists_collecter->tokens = tokens;
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
        tokens = expand(tokens, env);
        // print_tokens(tokens);
        // printf("after expand ======================\n");
        if (check_syntax_error(str, env) == 1 ||  ft_strlen(str) == 0 || ft_strncmp(str, ":", ft_strlen(str)) == 0 || check_syntax_error_tokens(tokens) == 1)
        {
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
        //print_minishell(minishell);
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
