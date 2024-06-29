/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 17:01:35 by akrid             #+#    #+#             */
/*   Updated: 2024/06/29 12:02:31 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "lib/libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

// colors
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define BLUE "\033[0;34m"
# define MAGENTA "\033[0;35m"
# define RESET "\033[0m"

typedef struct s_environment
{
	char						*key;
	char						*value;
	struct s_environment		*next;
}								t_environment;

// -------------- parsing -------------------------

typedef enum e_type_of_token
{
	T_WORD,
	T_PIPE,
	T_REDIRECTION_IN,
	T_REDIRECTION_OUT,
	T_REDIRECTION_APPEND,
	T_HERDOC,
	T_ENV_VARIABLE
}								t_type_of_token;

typedef struct s_token
{
	t_type_of_token				type;
	char						*value;
	struct s_token				*next;
	struct s_token				*prev;
}								t_token;

typedef struct s_files_redirection
{
	char						*filename;
	int							should_expand_heredoc;
	t_type_of_token				type;
	struct s_files_redirection	*next;
}								t_file_redirection;

typedef struct s_args
{
	char						*args;
	struct s_args				*next;
}								t_args;

typedef struct s_minishell
{
	char						*command;
	char						**args;
	char						*path;
	int							infile;
	int							outfile;
	int							cmd_order;
	int							nbr_cmd;
	int							*pipe;
	char						*heredoc_path;
	t_file_redirection			*files;
	struct s_minishell			*next;
}								t_minishell;

typedef struct s_minishell_data_help
{
	char						*command;
	char						*args1;
	char						**args;
	t_file_redirection			*files;
	int							new_command;
}								t_minishell_data_help;

typedef struct s_lists_collecter
{
	t_minishell					**minishell;
	t_token						**tokens;
	t_environment				**env;
	int							p;
}								t_lists_collecter;

extern t_lists_collecter		*lists_collecter;

// -------------- common function ---------------
void			set_exit_status(t_environment *env,
					int new_value);
int				args_count(char **args);
void			free_split(char **splited);
void			free_at_exit(void);
char			*ft_putnbr(int nbr);
void			swap_data(t_environment *helper);
void			sort(t_environment **sort_env);
void			print(char *key, char *value);
void			handle_heredoc_signals(int signal);
char			*expand_string(char *str, int from_heredoc);
int				is_directory(char *cmd);
int				empty_cmd(char *cmd);
int				parse_cmds(char *command);
char			**check_paths(t_environment *temp, char *cmd);

// --------------- signals ---------------------
void interactive_sigint(int sig);
void interactive_sigquit(int sig);
void active_sigint(int sig);
void active_sigquit(int sig);
void handle_signals(void (*sigint)(int), void (*sigquit)(int), void (*sigint_old)(int), void (*sigquit_old)(int));

// environment
t_environment	*env_node(char *key, char *value);
void			env_add_back(t_environment **env,
					t_environment *node);
int				env_size(t_environment *env);
t_environment	*get_before_target(t_environment *env,
					t_environment *target);
void			put_at_end(t_environment **env,
					char *target_key);
void			increment_shlvl(t_environment *env);
char			*create_paths(void);
t_environment	*creat_env(void);
void			get_environment(t_environment **env,
					char **base_env);
t_environment	*env_get_bykey(t_environment *env, char *key);
char			**convert_env(t_environment *env);
void			free_environment(t_environment *env);
void			free_copy(t_environment *env);
t_environment	*duplicate_env(t_environment *env);
t_environment	*extract_node(char *arg);
int				pars_node(t_environment *new, char *arg);
void			append_value(t_environment *env,
					t_environment *new);
void			replace_value(t_environment *env,
					t_environment *new);
					
// ------------------------ execution -------------------------
char			*get_cmd_path(char *cmd, t_environment *env,
					int i);
int				cmd_count(t_minishell *mini);
void			get_in_out_priorities(t_minishell *singl_mini);
void			unlink_files(t_minishell *minishell);

int				check_builtin(t_minishell *singl_mini,
					t_environment **env);
int				is_builtin(char *cmd);

int				check_delemeter(char *str, char *filename,
					int fd);
void			fill_heredoc(t_minishell *temp,
					t_file_redirection *files);
void			loop_heredoc(t_minishell *minishell);
int				fork_heredoc(t_minishell *minishell,
					t_environment *env);
int				check_heredoc(t_minishell *minishell,
					t_environment *env, int i);

int				file_error(t_minishell *minishell,
					t_environment *env, char *filename);
int				open_file_extended(t_minishell *minishell,
					t_environment *env,
					t_file_redirection *files);
int				open_files(t_minishell *minishell,
					t_environment *env,
					t_file_redirection *files);
void			start_execute_one(t_minishell *minishell,
					t_environment **env);
void			execute_one(t_minishell *minishell,
					t_environment **env);

void			pipe_init(t_minishell *mini);
void			close_pipes(int *pipe, int nbr_cmd);
void			unlink_files(t_minishell *minishell);

void			wait_childs(t_minishell *mini,
					t_environment *env, int num_cmd);
void			get_in_out_extended(t_minishell *singl_mini);
void			get_in_out_priorities(t_minishell *singl_mini);
void			final_execution(t_minishell *singl_mini,
					t_environment **env);
void			execute_all(t_minishell *minishell,
					t_environment **env);

void			restore_std_in_out(int stdout, int stdin);
char			*get_herdoc_path(int i);
void			minishell_init(t_minishell *minishell,
					int count_cmds);
void			set_under_score(t_minishell *minishell,
					t_environment **env);
void			execution(t_minishell *minishell,
					t_environment **env);

// builtin
void			add_node(t_environment *env,
					t_environment *new);
int				export_print(t_environment **env);
int				export_add(t_minishell *minishell,
					t_environment *env);
void			handel_input_output(t_minishell *singl_mini);
void			handel_exit_status(t_environment *env,
					int nbr_cmd, int exit_value);
void			cd(t_minishell *singl_mini, t_environment *env);
void			envi(t_minishell *singl_mini,
					t_environment *env);
void			fake_exit(t_minishell *singl_mini,
					t_environment *env);
void			pwd(t_minishell *singl_mini,
					t_environment *env);
void			unset(t_minishell *minishell,
					t_environment **env);
void			export(t_minishell *minishell,
					t_environment **env);
void			echo(t_minishell *single_mini,
					t_environment *env);

#endif
