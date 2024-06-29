/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtamouss <rtamouss@student.42.fr>          +#+  +:+       +#+        */
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


// PARSIIIIIIIIIIIIIIIIIIIIING

void				check_quote(const char *str, int *in_single_quotes, int *in_double_quotes, int *i);
void				update_quote_state(int *in_quote, char *quote, char c);
int					check_if_have_quotes(char *str);
int					check_unclosed_quotes(const char *str);
void				update_nb_quote(char c, int *nb_quote_single, int *nb_quote_double);
void				insert_token(t_token **head, t_token *new_token, t_token *target);
void				add_token_back(t_token **head, t_token *new_token);
char				*get_type_token(t_type_of_token type);
void				print_tokens(t_token *tokens);
int					tokens_size(t_token *tokens);
t_token				get_last_token(t_token *tokens);
void				remove_token(t_token **head, t_token *target);
t_token				*new_token(t_type_of_token type, char *value);
void				add_token_front(t_token **head, t_token *new_token);
void				find_target(t_token **head, t_token *target, t_token **temp, t_token **prev);
int					is_operator(char c);
int					is_pipe(char c);
int					is_redirection_in(char c);
int					is_redirection_out(char c);
int					is_quote(char c);
int					is_env_variable(char c);
int					is_arithmetic_operator(char c);
int					is_colone(char c);
int					is_number(char c);
int					is_alpha(char c);
t_file_redirection	*new_file_redirection(char *filename, t_type_of_token type);
void				add_file_redirection_back(t_file_redirection **head, t_file_redirection *new_file);
void				print_files(t_minishell *temp);
void				handle_operator(t_token **head, const char *str, int *i);
void				handle_word(t_token **head, const char *str, int *i);
t_token				*tokenize_input(const char *str);
int					is_whitespace(char c);
int					check_syntax_error(const char *str);
void				puterr(char *str);
int					check_syntax_error_tokens_helper2(t_token *temp);
int					check_syntax_error_tokens_helper(t_token *temp);
int					check_syntax_error_tokens(t_token *tokens);
int					s_minishell_size(t_minishell *minishell);
t_minishell			*new_minishell(char *command, char **args, t_file_redirection *files);
void				add_minishell_back(t_minishell **head, t_minishell *new_minishell);
void				print_minishell(t_minishell *minishell);
void				*new_arg(char *arg);
void				add_arg_back(t_args **head, t_args *new_arg);
int					count_args(char **args);
void				join_args(t_token *temp, char **args1, char ***args);
void				print_args(t_minishell *temp);
void				free_at_exit();
void				free_heredocs(t_file_redirection *files);
void				free_minishell(t_minishell *minishell);
void				free_files(t_file_redirection *files);
void				free_environment(t_environment *env);
void				initialize_value_split2(int *j, char *quote, int *in_quote);
void				ft_skip2(int *i, int in_quote, const char *s, char c);
char				**ft_split2_helper(char **res, char const *s, char c, int i);
char				**ft_split2(char const *s, char c);
char				**ft_free(char **strs);
void				free_tokens(t_token *tokens);
int					ft_count_words(char const *str, char sep);
void				free_args(char **args);
void				init_expand_string(int *i, int *in_single_quotes, int *in_double_quotes, char **env_variable);
char				*expand_string(char *str, int from_heredoc);
void				split_expanded_string(t_token **temp, t_token **tokens);
int					expand_string_helper(t_token **tokens, t_token **temp);
int					is_word(char c);
void				find_target(t_token **head, t_token *target, t_token **temp, t_token **prev);
int					check_whitespaces(const char *str);
void				handle_quotes_after_dollar(char *str);
char				*change_value(char *str, int *i, int *index, char *env_variable);
int					expand_helper(int index, int *i, char *str);
int					ambiguouse_redirect(char *old);
int					ft_remove_char(char *str, int index);
void				delete_quotes_from_string(char *str);
t_token				*expand(t_token *tokens);;
void				delete_quotes_from_files(t_minishell *minishell);
void				delete_quotes_from_args(t_minishell *minishell);
t_minishell			*delete_quotes(t_minishell *minishell);
int					in_quote(const char *str);
t_token				*add_redirection_in(t_token *temp, t_file_redirection **files);
t_token				*add_redirection_out(t_token *temp, t_file_redirection **files);
t_token				*add_redirection_append(t_token *temp, t_file_redirection **files);
t_token				*add_heredoc(t_token *temp, t_file_redirection **files);
int					initialize_and_free(t_minishell_data_help *data);
int					handle_word_new_command(t_minishell_data_help *data, t_token *temp);
void				token_to_minishell_helper(t_minishell_data_help *data, t_token **temp);
void				initialize_token_to_minishell_data(t_minishell_data_help *data);
t_minishell			*token_to_minishell(t_token *tokens);
int					is_empty(char *str);
void				handle_ctrl_c(int signal);
int					check_if_have_space(const char *str);
void				handle_sigquit(int signal);
int					check_delemeter_for_sr(char *str, char *filename);
void				fill_heredoc_for_sr(t_file_redirection *heredoc);
void				loop_heredoc_for_sr(t_file_redirection *heredocs);
void				fill_error_codes(char **error_codes);
int					help_heredoc_sr(int  *heredoc_counter, t_token *tokens, int error_code, t_file_redirection **heredocs);
int					check_heredoc_for_syntax_error(t_file_redirection **heredocs, t_token *tokens, int error_code);
void				print_heredocs(t_file_redirection *files);
void				collecter_init(t_minishell **minishell, t_environment **env, t_token **tokens);
int					not_alpha_numeric(char c);

#endif