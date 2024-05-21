/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtamouss <rtamouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 17:01:35 by akrid             #+#    #+#             */
/*   Updated: 2024/05/07 09:37:43 by rtamouss         ###   ########.fr       */
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