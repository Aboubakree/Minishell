/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtamouss <rtamouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:18:34 by akrid             #+#    #+#             */
/*   Updated: 2024/05/05 17:8:135:21rtamouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// syntax error checking

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

int check_syntax_error(const char *str)
{
    if (check_unclosed_quotes(str) == 1)
        return (printf("Syntax error : Unclosed quote\n"), 1);
    if (check_misplaced_operators(str) == 1)
        return (1);
    if (check_logical_operators(str) == 1)
        return (1);
    if (check_invalid_redirection(str) == 1)
        return (1);
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

int is_word(char c)
{
    return (!is_operator(c) && !is_quote(c) && !is_whitespace(c) && !is_env_variable(c));
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
t_file_redirection *new_file_redirection(char *filename, t_type_of_token type)
{
    t_file_redirection *file;

    file = (t_file_redirection *)malloc(sizeof(t_file_redirection));
    if(!file)
        return (NULL);
    file->filename = filename;
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
        printf("type: %s, value: %s\n", get_type_token(tokens->type), tokens->value);
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
    char *temp = ft_substr(str, j, *i - j);
    // add_token_back(head, new_token(T_WORD, ft_substr(str, j, *i - j)));
    add_token_back(head, new_token(T_WORD, temp));
    // free(temp);
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
        while(ft_strchr(" \t\n\v\f\r", str[i]))
            i++;
        if (ft_strchr("<>|", str[i]))
            handle_operator(&head, str, &i);
        else
            handle_word(&head, str, &i);
        i++;
    }
    return (head);
}

void free_tokens(t_token *tokens)
{
    t_token *temp;

    if (tokens == NULL)
        return ;
    while(tokens)
    {
        temp = tokens;
        tokens = tokens->next;
        free(temp->value);
        free(temp);
    }
}

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
    minishell->command = command;
    minishell->args = args;
    minishell->path = NULL;
    minishell->infile = 0;
    minishell->outfile = 1;
    minishell->first_cmd = 0;
    minishell->last_cmd = 0;
    // minishell->delimiter = delimiter;
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
t_token *expand(t_token *tokens, t_environment *env)
{
    t_token *temp = tokens;
    int in_single_quotes;
    int in_double_quotes;
    int i;

    in_single_quotes = 0;
    in_double_quotes = 0;
    i = 0;
    while(temp)
    {
        if (temp->type == T_WORD)
        {
            if (ft_strchr(temp->value, '$') == NULL)
            {
                temp = temp->next;
                continue;
            }
            i = 0;
            while(temp->value[i])
            {
                if (temp->value[i] == '\'' && in_single_quotes == 0 && in_double_quotes == 0)
                    in_single_quotes = 1;
                else if (temp->value[i] == '\'' && in_single_quotes == 1)
                    in_single_quotes = 0;
                else if (temp->value[i] == '\"' && in_double_quotes == 0 && in_single_quotes == 0)
                    in_double_quotes = 1;
                else if (temp->value[i] == '\"' && in_double_quotes == 1)
                    in_double_quotes = 0;
                if (temp->value[i] == '$' && in_single_quotes == 0)
                {
                    if (is_whitespace(temp->value[i + 1]) || temp->value[i + 1] == '\0')
                    {
                        i++;
                        continue;
                    }
                    int index = i;
                    index++;
                    while(temp->value[index] && is_word(temp->value[index]))
                        index++;
                    char *env_variable = ft_substr(temp->value, i + 1, index - i - 1);
                    t_environment *get_env = env_get_bykey(env, env_variable);
                    char *value;
                    if(get_env == NULL)
                        value = ft_strdup("");
                    else
                        value = get_env->value;
                    char *first = ft_substr(temp->value, 0, i);
                    char *last = ft_strdup(&temp->value[index]);
                    char *new_value = ft_strjoin(first, value);
                    new_value = ft_strjoin(new_value, last);
                    temp->value = new_value;
                    // free(first);
                    // free(last);
                    // free(env_variable);
                    // last = NULL;
                    // first = NULL;
                    // env_variable = NULL;
                }
                i++;
            }
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
    if (temp->args == NULL)
        return (minishell);
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

// t_minishell *delete_quotes(t_minishell *minishell)
// {
//     t_minishell *temp;
//     int in_single_quote, in_double_quote;
//     int i, j, len;
//     char *new_arg;

//     temp = minishell;
//     while (temp)
//     {
//         i = 0;
//         while (temp->args[i])
//         {
//             in_single_quote = 0;
//             in_double_quote = 0;
//             j = 0;
//             len = 0;
//             new_arg = malloc(sizeof(char));
//             new_arg[0] = '\0';
//             while (temp->args[i][j])
//             {
//                 if (temp->args[i][j] == '\'' && !in_double_quote)
//                     in_single_quote = !in_single_quote;
//                 else if (temp->args[i][j] == '\"' && !in_single_quote)
//                     in_double_quote = !in_double_quote;
//                 else
//                 {
//                     len++;
//                     new_arg = realloc(new_arg, len + 1);
//                     new_arg[len - 1] = temp->args[i][j];
//                     new_arg[len] = '\0';
//                 }
//                 j++;
//             }
//             free(temp->args[i]);
//             temp->args[i] = new_arg;
//             i++;
//         }
//         temp = temp->next;
//     }
//     return (minishell);
// }


// t_minishell *delete_quotes(t_minishell *minishell)
// {
//     t_minishell *temp;
//     int i;
//     int j;
//     char quote;
//     int in_quote;

//     temp = minishell;

//     i = 0;
//     quote = '\0';
//     in_quote = 0;
//     while(temp)
//     {
//         j = 0;
//         while(temp->args[j])
//         {
//             i = 0;
//             update_quote_state(&in_quote, &quote, temp->args[j][i]);
//             while(temp->args[j][i])
//             {
//                 update_quote_state(&in_quote, &quote, temp->args[j][i]);
                
//                 i++;
//             }
//             j++;
//         }
//         temp = temp->next;
//     }
//     return (minishell);
// }



t_minishell *token_to_minishell(t_token *tokens)
{
    t_minishell *minishell;
    char *command = NULL;
    char *args1 = NULL;
    char **args = NULL;
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
            args = NULL;
            args1 = NULL;
            files = NULL;
            temp = temp->next;
            new_command = 1;
            continue;
        }
        if (temp->type == T_WORD)
        {
            char *temp_args1;
            if (new_command == 1)
            {
                temp_args1 = ft_strjoin(args1, temp->value);
                free(args1);
                args1 = temp_args1;
                command = ft_strdup(temp->value);
                temp_args1 = ft_strjoin(args1, " ");
                free(args1);
                args1 = temp_args1;
                args = ft_split2(args1, ' ');
                new_command = 0;
            }
            else
            {
                temp_args1 = ft_strjoin(args1, temp->value);
                free(args1);
                args1 = temp_args1;
                temp_args1 = ft_strjoin(args1, " ");
                free(args1);
                args1 = temp_args1;
                args = ft_split2(args1, ' ');
            }
            
        }
        else if (temp->type == T_REDIRECTION_IN)
        {
            temp = temp->next;
            add_file_redirection_back(&files, new_file_redirection(ft_strdup(temp->value), T_REDIRECTION_IN));
        }
        else if (temp->type == T_REDIRECTION_OUT)
        {
            temp = temp->next;
            add_file_redirection_back(&files, new_file_redirection(ft_strdup(temp->value), T_REDIRECTION_OUT));
        }
        else if (temp->type == T_REDIRECTION_APPEND)
        {
            temp = temp->next;
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
void print_minishell(t_minishell *minishell)
{
    t_minishell *temp;
    // t_args *temp_args;

    temp = minishell;
    int i = 0;
    while(temp)
    {
        printf("--------------------\n");
        printf("command [%d]: %s\n",i,  temp->command);
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
        if (temp->files)
        {
            t_file_redirection *temp_files = temp->files;
            printf("files :\n");
            while(temp_files)
            {
                printf(" file: %s \n", temp_files->filename);
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
        // new prompt
        rl_clear_message();
        // rl_on_new_line();
        printf("\n");
        printf("%sminishell$ %s", GREEN, RESET);
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
    t_file_redirection *temp_files;

    if (minishell == NULL)
        return ;
    while(minishell)
    {
        temp = minishell;
        minishell = minishell->next;
        if (temp->command)
            free(temp->command);
        if (temp->path != NULL)
            free(temp->path);
        if (temp->args)
            free_split(temp->args);
        while(temp->files)
        {
            temp_files = temp->files;
            temp->files = temp->files->next;
            free(temp_files->filename);
            free(temp_files);
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
    // if (ft_strncmp("echo", singl_mini->command, 5) == 0)
    //    return (echo(singl_mini, *env), 0);
    if (ft_strncmp("env", singl_mini->command, 4) == 0)
        return (envi(*env), 0);
    if (ft_strncmp("pwd", singl_mini->command, 4) == 0 )
        return (pwd(*env), 0);
    // if (ft_strncmp("export", singl_mini->command, 7) == 0)
    //     return (export(singl_mini, *env), 0);
    if (ft_strncmp("unset", singl_mini->command, 6) == 0)
        return (unset(singl_mini, env), 0);
    if (strncmp("exit", singl_mini->command, 5) == 0)
        return (fake_exit(singl_mini), 0);
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

void fill_heredoc(t_file_redirection *file)
{
    int     fd;
    char    *str;

    fd = open("heredoc_buffer", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
        perror("open");
    while (1)
    {
        str = readline(">");
        if (ft_strncmp(file->filename, str, ft_strlen(str) + 1) == 0)
        {
            free(str);
            close(fd);
            break;
        }
        write(fd, str, ft_strlen(str));
        write(fd, "\n", 1);
        if(str)
            free(str);
    }
    exit(0);
}

void check_heredoc(t_minishell *minishell)
{
    pid_t   pid;
    t_file_redirection *files;

    files = minishell->files;
    while (files)
    {
        if (files->type == T_HERDOC)
        {
            pid = fork();
            if (pid == 0)
                fill_heredoc(files);
            wait(NULL);
        }
        files = files->next;
    }
}

void file_error(char *filename)
{
    write(2, "bash: ",6);
    write(2, filename, ft_strlen(filename));
    write(2, ": ", 2);
    perror("");
    exit(1);
}

void open_files(t_minishell *minishell)
{
    t_file_redirection *files;

    check_heredoc(minishell);
    files = minishell->files;
    while (files)
    {
        if (files->type == T_REDIRECTION_IN)
        {
            if (minishell->infile != 0)
                close(minishell->infile);
            minishell->infile = open(files->filename, O_RDONLY, 0644);
            if (minishell->infile < 0)
                file_error(files->filename);
        }
        else if (files->type == T_REDIRECTION_OUT)
        {
            if (minishell->outfile != 1)
                close(minishell->outfile);
            minishell->outfile = open(files->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (minishell->outfile < 0)
                file_error(files->filename);
        }
        else if (files->type == T_REDIRECTION_APPEND)
        {
            if (minishell->outfile != 1)
                close(minishell->outfile);
            minishell->outfile = open(files->filename, O_WRONLY | O_APPEND | O_CREAT, 0664);
            if (minishell->outfile < 0)
                file_error(files->filename);
        }
        else if (files->type == T_HERDOC)
        {
            if (minishell->infile != 0)
                close(minishell->infile);
            minishell->infile = open("heredoc_buffer", O_RDONLY, 0644);
            if (minishell->infile < 0)
                file_error(files->filename);
        }
        files = files->next;
    }
}


void    start_execute_one(t_minishell *minishell, t_environment **env)
{
    open_files(minishell);
    if (minishell->command == NULL)
        exit(0);
    minishell->path = get_cmd_path(minishell->command, *env, 0);
    if (minishell->path == NULL)
        exit(127);
    dup2(minishell->infile, 0);
    dup2(minishell->outfile, 1);
    if (execve(minishell->path, minishell->args, NULL))
        perror("execve");
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
    status = status >> 8;
    //$? = status
    // printf("exit_stat : %d\n", status);
}


// --------------------------------------- end of exe one cmd ----------------------------------------------


//----------------------------------------- multiple commands --------------------------------------


void    pipe_init(t_minishell *mini)
{
    if (pipe(mini->pipe))
    {
        perror("pipe");
        free_minishell(mini);
        exit(1);
    }
}

void wait_childs(t_minishell *mini, int num_cmd)
{
    int i;
    int status;

    i = 0;
    close(mini->pipe[0]);
    close(mini->pipe[1]);
    while (i < num_cmd)
    {
        if (wait(&status) > 0)
        {
            status = status >> 8;
            //? = status
            printf("exit_stat : %d\n", status);
            i ++;
        }
        else
        {
            perror("wait");
            i ++;
        }
    }
}

void get_in_out_priorities(t_minishell *singl_mini)
{
    if (singl_mini->first_cmd == 1)
    {
        close(singl_mini->pipe[0]);
        dup2(singl_mini->infile, 0);
        if (singl_mini->outfile == 1)
            dup2(singl_mini->pipe[1], 1);
        else
            dup2(singl_mini->outfile, 1);
    }
    else if (singl_mini->last_cmd == 1)
    {
        close(singl_mini->pipe[1]);
        if (singl_mini->infile == 0)
            dup2(singl_mini->pipe[0], 0);
        else
            dup2(singl_mini->infile, 0);
        dup2(singl_mini->outfile, 1);
    }
    else
    {
        printf("ana ls tanya\n");
        // dup2(singl_mini->pipe[0], 0);
        dup2(singl_mini->pipe[1], STDOUT_FILENO);
    }
}




void final_execution(t_minishell *singl_mini, t_environment **env)
{
    open_files(singl_mini);
    get_in_out_priorities(singl_mini);
    if (check_builtin(singl_mini, env) == 0)
        exit (0);
    singl_mini->path = get_cmd_path(singl_mini->command, *env, 0);
    if (singl_mini->path == NULL)
        exit(127);
    execve(singl_mini->path, singl_mini->args, NULL);
    perror("execve");
    exit(1);
}

void execute_all(t_minishell *minishell, t_environment **env, int num_cmd)
{
    pid_t       pid;
    t_minishell *temp;
    
    temp = minishell;
    pipe_init(minishell);
    while (temp)
    {
        pid = fork();
        if (pid == 0)
        {
            if (temp == minishell)
                temp->first_cmd = 1;
            if (temp->next == NULL)
                temp->last_cmd = 1;
            final_execution(temp, env);
        }
        temp = temp->next;
    }
    wait_childs(minishell, num_cmd);
}


//----------------------------------------- end of multiple command --------------------------------


void    execution(t_minishell *minishell, t_environment **env)
{
    if (cmd_count(minishell) == 1)
    {
        if (check_builtin(minishell, env) == 0)
            return ;
        else
            execute_one(minishell, env);
    }
    else
        execute_all(minishell, env, cmd_count(minishell));
}


int main(int argc, char **argv, char **base_env)
{
    t_environment *env;
    t_token *tokens;
    t_minishell *minishell;
    char *str;
    
    (void)argv;
    env = NULL;
    if (argc > 1)
        return (1);
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
        str = readline("\033[0;32mminishell$ \033[0m");
        if (str == NULL)
            break;
        tokens = tokenize_input(str);
        tokens = expand(tokens, env);
        if (check_syntax_error(str) == 1 ||  ft_strlen(str) == 0 || check_syntax_error_tokens(tokens) == 1)
        {
            add_history(str);
            free(str);
            continue;
        }
        minishell = token_to_minishell(tokens);
        minishell = delete_quotes(minishell);
        execution(minishell, &env);
        if (str[0] != '\0')
            add_history(str);
        free(str);
        free_tokens(tokens);
        free_minishell(minishell);
    }
    free_environment(env);
    return (0);
}

