/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtamouss <rtamouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 17:01:35 by akrid             #+#    #+#             */
/*   Updated: 2024/06/09 22:11:46 by rtamouss         ###   ########.fr       */
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
#include <sys/wait.h>
#include <sys/types.h>
#include <readline/history.h>
#include <fcntl.h>
#include <sys/stat.h>

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

typedef struct s_files_redirection
{
    char *filename;
    int should_expand_heredoc;
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
    char *path;
    int infile;
    int outfile;
    int cmd_order;
    int nbr_cmd;
    int *pipe;
    char *heredoc_path;
    t_file_redirection *files; 
    struct s_minishell *next;
} t_minishell;

typedef struct s_minishell_data_help
{
	char	*command;
	char	*args1;
	char	**args;
	t_file_redirection	*files;
	int	new_command;
} t_minishell_data_help;

typedef struct s_lists_collecter
{
    t_minishell **minishell;
    t_token **tokens;
    t_environment **env;
} t_lists_collecter;


extern t_lists_collecter *lists_collecter;



// -------------- common function ---------------
int args_count(char **args);
void free_split(char **splited);
void free_at_exit();
char *ft_putnbr(int nbr);
void set_exit_status(t_environment *env, int new_value);


// --------------------------------------------------- parsing ---------------------------------------------------



//environment
t_environment *env_node(char *key, char *value);
void    env_add_back(t_environment **env, t_environment *node);
t_environment *get_before_target(t_environment *env, t_environment *target);
void  put_at_end(t_environment **env, char *target_key);
void get_environment(t_environment **env, char **base_env);
t_environment *env_get_bykey(t_environment *env, char *key);
char **convert_env(t_environment *env);
void free_environment(t_environment *env);



// ------------------------ execution -------------------------
char	*get_cmd_path(char *cmd, t_environment *env, int i);
int cmd_count(t_minishell *mini);
int open_files(t_minishell *minishell, t_environment *env, t_file_redirection *files);
void get_in_out_priorities(t_minishell *singl_mini);
void unlink_files(t_minishell *minishell);



// builtin
void    handel_input_output(t_minishell *singl_mini);
void handel_exit_status(t_environment *env, int nbr_cmd , int exit_value);
void cd(t_minishell *singl_mini, t_environment *env);
void    envi(t_minishell *singl_mini, t_environment *env);
void fake_exit(t_minishell *singl_mini, t_environment *env);
void    pwd(t_minishell *singl_mini, t_environment *env);
void unset(t_minishell *minishell, t_environment **env);
void export(t_minishell *minishell, t_environment **env);
void echo(t_minishell *single_mini, t_environment *env);


#endif
