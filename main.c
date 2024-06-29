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

int syntax_checker(t_token *tokens, char *str)
{
	if (check_syntax_error(str) != 0 || tokens == NULL)
	{
		free_tokens(tokens);
		return (1);
	}
	if (check_syntax_error_tokens(tokens) != 0 
		|| tokens == NULL ||  ft_strlen(str) == 0 
		|| ft_strncmp(str, ":", ft_strlen(str)) == 0)
	{
		t_file_redirection *heredocs;
		heredocs = NULL;
		int error_code = check_syntax_error_tokens(tokens);
		check_heredoc_for_syntax_error(&heredocs, tokens, error_code);
		free_heredocs(heredocs);
		free_tokens(tokens);
		return (1);
	}
	return (0);
}

void minishell_loop(t_environment **env, t_token **tokens, t_minishell **minishell)
{
	char *str;

	while (1)
	{

		str = readline("$> ");
		if (str == NULL)
			break;
		if (str[0] != '\0' && check_whitespaces(str))
			add_history(str);
		*tokens = tokenize_input(str);
		*tokens = expand(*tokens);
		if (check_whitespaces(str) == 0 || syntax_checker(*tokens, str))
		{
			free(str);
			continue;
		}

		*minishell = token_to_minishell(*tokens);
		*minishell = delete_quotes(*minishell);
		execution(*minishell, env);
		free(str);
		free_tokens(*tokens);
		free_minishell(*minishell);
	}
}

int main(int argc, char **argv, char **base_env)
{
	t_environment *env;
	t_token *tokens;
	t_minishell *minishell;
	
	(void)argv;
	if (argc > 1)
		return (1);
	collecter_init(&minishell, &env, &tokens);
	get_environment(&env, base_env);
	printf("Welcome to minishell\n");
	handle_signals(interactive_sigint, SIG_IGN,  SIG_IGN,  SIG_IGN);
	minishell_loop(&env, &tokens, &minishell);
	free_environment(env);
	free(lists_collecter);
	printf("exit\n");
	return (0);
}