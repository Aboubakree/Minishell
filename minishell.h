/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 17:01:35 by akrid             #+#    #+#             */
/*   Updated: 2024/05/03 15:11:56 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <string.h>
#include <errno.h>
#include "libft/libft.h"

typedef struct s_environment
{
    char *key;
    char *value;
    struct s_environment *next;
} t_environment;


typedef struct s_token
{
    char    *str;
    struct s_token *next;
} t_token;

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