/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtamouss <rtamouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:18:34 by akrid             #+#    #+#             */
/*   Updated: 2024/05/05 14:55:11 by rtamouss         ###   ########.fr       */
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

int check_unclosed_quotes(const char *str)
{
    int i;
    int in_single_quotes;
    int in_double_quotes;

    i = 0;
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

int is_whitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

int check_misplaced_operators(const char *str)
{
    int i;
    char *pipe;
    
    i = 0;
    while(str[i])
    {
        while(is_whitespace(str[i]))
            i++;
        if (str[i] == '|')
            return (printf("Syntax error : Misplaced pipe\n"), 1);
        pipe = ft_strrchr(str, '|');
        while(is_whitespace(*pipe))
            pipe++;
        if (*pipe == '\0')
            return (printf("Syntax error : Misplaced pipe\n"), 1);
        i++;
    }
    return (0); 
}

int check_syntax_error(const char *str)
{
    if (check_unclosed_quotes(str) == 1)
        return (printf("Syntax error : Unclosed quote\n"), 1);
    // if (check_misplaced_operators(str) == 1)
    //     return (1);
    return (0);
}

// syntax error check

int main(int argc, char **argv, char **base_env)
{
    t_environment *env;
    // t_token       *temp;
    
    (void)argv;
    env = NULL;
    if (argc > 1)
        return (1);
    get_environment(&env, base_env);
    char *str;
    printf("Welcome to minishell\n");
    while (1)
    {
        str = readline("minishell$ ");
        if (str == NULL)
            break;
        if (check_syntax_error(str) == 1)
        {
            free(str);
            continue;
        }
        printf("%s\n", str);
        free(str);
    }
    return (0);
}
