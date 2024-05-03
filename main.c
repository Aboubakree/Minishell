/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:18:34 by akrid             #+#    #+#             */
/*   Updated: 2024/05/03 20:14:33 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_token(t_token **lst)
{
    t_token *temp;

    temp = *lst;
    while (*lst)
    {
        temp = *lst;
        *lst = (*lst)->next;
        free(temp->str);
        free(temp);
    }
    *lst = NULL;
}

t_token *token_node(char *str)
{
    t_token *new;

    new = malloc(sizeof(t_token));
    if (new == NULL)
        return (NULL);
    new->str = str;
    new->next = NULL;
    return (new);
}
void token_push(t_token **lst, t_token *node)
{
    t_token *temp;

    temp = *lst;
    if (*lst == NULL)
        *lst = node;
    else
    {
        while (temp->next)
            temp = temp->next;
        temp->next = node;
    }
}

int fetch_quots(char *str, char c)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while (str && str[i])
    {
        if (str[i] == c)
            count ++;
        i ++;
    }
    if (str)
        free(str);
    return (count);
}

void lexing(t_token **tokens, char *input)
{
    char *pipe;
    char *last_pipe;
    int s_quots;
    int d_quots;

    pipe = input;
    last_pipe = input;
    while (pipe != NULL)
    {
        pipe = ft_strchr(pipe, '|');
        if (pipe != NULL)
        {
            s_quots = fetch_quots(ft_substr(last_pipe, 0, pipe - last_pipe), '\'');
            d_quots = fetch_quots(ft_substr(last_pipe, 0, pipe - last_pipe), '"');
            if (s_quots % 2 == 0 && d_quots % 2 == 0)
            {
                token_push(tokens, token_node(ft_substr(last_pipe, 0, pipe - last_pipe)));
                last_pipe = pipe;
            }
            pipe ++;
        }
        if (last_pipe != input && pipe)
            last_pipe ++;
    }
    token_push(tokens, token_node(ft_substr(last_pipe, 0, pipe - last_pipe)));
}


int main(int argc, char **argv, char **base_env)
{
    t_environment *env;
    t_token       *tokens;
    t_token       *temp;
    
    (void)argv;
    env = NULL;
    tokens = NULL;
    if (argc > 1)
        return (1);
    get_environment(&env, base_env);
    char *str;
    while (1)
    {
        str = readline("minishell-->$");
        if (str == NULL)
            break;
        lexing(&tokens, str);
        temp = tokens;
        while (temp)
        {
            printf("%s\n", temp->str);
            temp = temp->next;
        }
        free_token( &tokens);
        free(str);
    }
    return (0);
}
