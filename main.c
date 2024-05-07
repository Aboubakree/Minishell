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
    if (str[i] == '|')
        return (printf("Syntax error : Misplaced Operator at first of line\n"), 1);
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
    add_token_back(head, new_token(T_WORD, ft_substr(str, j, *i - j)));
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
        if(ft_strchr(" \t\n\v\f\r", str[i]))
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

    if (!tokens)
        return ;
    while(tokens)
    {
        temp = tokens;
        tokens = tokens->next;
        free(temp->value);
        free(temp);
    }
}
// end tokenization
t_token *tokens;
void handle_ctrl_c(int signal)
{
    if (signal == SIGINT)
    {
        printf(" will be freed \n");
        free_tokens(tokens);
        exit(1);
    }
}
int main(int argc, char **argv, char **base_env)
{
    t_environment *env;
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
            exit(1);
        }
        str = readline("minishell$ ");
        if (str == NULL)
            break;
        tokens = tokenize_input(str);
        print_tokens(tokens);

        if (check_syntax_error(str) == 1)
        {
            free(str);
            continue;
        }
        printf("%s\n", str);
    }
    return (0);
}
