/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtamouss <rtamouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:18:34 by akrid             #+#    #+#             */
/*   Updated: 2024/06/10 20:13:09 rtamouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
// syntax error checking/

t_lists_collecter *lists_collecter;

// end tokenization
void	handle_ctrl_c(int signal)
{
	if (signal == SIGINT)
	{
		// clear the current line	
		// rl_replace_line("", 0);
		// move to a new line
		printf("\n");
		//display the prmmpt on the new line
		rl_on_new_line();
		// redrws the readline line
		rl_redisplay();
		set_exit_status(*(lists_collecter->env), 130);
		return;
	}
}

void	handle_sigquit(int signal)
{
	if (signal == SIGQUIT)
	{
		return ;
	}
}

int main(int argc, char **argv, char **base_env)
{
	t_environment	*env;
	t_token			*tokens;
	t_minishell		*minishell;
	char			*str;

	(void)argv;
	if (argc > 1)
		return (1);
	env = NULL;
	tokens = NULL;
	minishell = NULL;
	collecter_init(&minishell, &env, &tokens);
	get_environment(&env, base_env);
	printf("Welcome to minishell\n");
	while (1)
	{
		if (signal(SIGINT, handle_ctrl_c) == SIG_ERR)
		{
			perror("signal");
			return (1);
		}
		else if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		{
			perror("signal");
			return (1);
		}
		str = readline("$> ");
		if (str == NULL)
			break ;
		if (check_whitespaces(str) == 0)
		{
			free(str);
			continue ;
		}
		if (str[0] != '\0')
			add_history(str);
		/* printf("before expand ======================\n"); */
		tokens = tokenize_input(str);
		// print_tokens(tokens);
		// printf("after expand ======================\n");
		tokens = expand(tokens);
		// print_tokens(tokens);
		// printf("after expand ======================\n");
		if (check_syntax_error(str) != 0 || tokens == NULL)
		{
			add_history(str);
			free(str);
			free_tokens(tokens);
			continue ;	
		}
		if (check_syntax_error_tokens(tokens) != 0 || tokens == NULL ||  ft_strlen(str) == 0 || ft_strncmp(str, ":", ft_strlen(str)) == 0)
		{
			t_file_redirection *heredocs;
			heredocs = NULL;
			int error_code = check_syntax_error_tokens(tokens);
			check_heredoc_for_syntax_error(&heredocs, tokens, error_code);
			free_heredocs(heredocs);
			add_history(str);
			free(str);
			free_tokens(tokens);
			continue;
		}
		minishell = token_to_minishell(tokens);
		// print_minishell(minishell);
		// printf("minishell after deleting quotes\n");
		minishell = delete_quotes(minishell);
		execution(minishell, &env);
		free(str);
		free_tokens(tokens);
		free_minishell(minishell);
	}
	free_environment(env);
	free(lists_collecter);
	return (0);
}