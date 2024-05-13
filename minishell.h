/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtamouss <rtamouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 17:01:35 by akrid             #+#    #+#             */
/*   Updated: 2024/05/13 17:42:43 by rtamouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include "lib/libft/libft.h"

// colors 
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define RESET "\033[0m"

typedef struct s_environment
{
    char *key;
    char *value;
    struct s_environment *next;
} t_environment;

// --------------------------------------------------- parsing ---------------------------------------------------

typedef enum e_type_of_token
{
    T_WORD,
    T_PIPE,
    T_REDIRECTION_IN,
    T_REDIRECTION_OUT,
    T_REDIRECTION_APPEND,
    T_HERDOC,
    T_ENV_VARIABLE
} t_type_of_token;

typedef struct s_token
{
    t_type_of_token type;
    char *value;
    struct s_token *next;
} t_token;

// typedef struct s_ast_node
// {
//     t_type_of_token type;
//     char *args;
//     struct s_ast_node *left;
//     struct s_as_node *right;
// } t_ast_node;
typedef struct s_files_redirection
{
    char *filename;
    t_type_of_token type;
    struct s_files_redirection *next;
} t_file_redirection;

typedef struct s_args
{
    char *args;
    struct s_args *next;
} t_args;

typedef struct s_minishell
{
    char *command;
    char **args;
    // t_args *args;
    char *path;
    t_file_redirection *files; 
    struct s_minishell *next;
} t_minishell;

// --------------------------------------------------- parsing ---------------------------------------------------
//environment
t_environment *env_node(char *key, char *value);
void    env_add_back(t_environment **env, t_environment *node);
void get_environment(t_environment **env, char **base_env);
t_environment *env_get_bykey(t_environment *env, char *key);


// builtin
void    cd(char *path, t_environment *env);
void    pwd(t_environment *env);
void    envi(t_environment *env);
void    fake_exit(char *arg);


#endif
