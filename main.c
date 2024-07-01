/* ************************************************************************** */
/*                                                                            */

/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:18:34 by akrid             #+#    #+#             */
/*   Updated: 2024/06/30 18:25:02 by akrid            ###   ########.fr       */

/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/history.h>
#include <readline/readline.h>

// syntax error checking/

t_lists_collecter	*g_lists_collecter;

int	syntax_checker(t_token *tokens, char *str)
{
  t_file_redirection	*heredocs;
	int					error_code;
  if (tokens == NULL)
		return (free_tokens(tokens), 1);
	if (check_syntax_error(str) != 0)
	{
		set_exit_status(*g_lists_collecter->env, 2);
		return (1);
	}
	if (check_syntax_error_tokens(tokens) != 0 || tokens == NULL
		|| ft_strlen(str) == 0 || ft_strncmp(str, ":", ft_strlen(str)) == 0)
	{
		heredocs = NULL;
		error_code = check_syntax_error_tokens(tokens);
		check_heredoc_for_syntax_error(&heredocs, tokens, error_code);
		free_heredocs(heredocs);
		set_exit_status(*g_lists_collecter->env, 2);
		return (1);
	}
	return (0);
}

void	minishell_loop(t_environment **env, t_token **tokens,
		t_minishell **minishell)
{
	char	*str;

	while (1)
	{
		str = readline("$> ");
		if (str == NULL)
			break ;
		if (str[0] != '\0' && check_whitespaces(str))
			add_history(str);
		*tokens = tokenize_input(str);
		*tokens = expand(*tokens);
		if (check_whitespaces(str) == 0 || syntax_checker(*tokens, str))
		{
			free(str);
			free_tokens(*tokens);
			continue ;
		}
		*minishell = token_to_minishell(*tokens);
		*minishell = delete_quotes(*minishell);
		execution(*minishell, env);
		free(str);
		free_tokens(*tokens);
		free_minishell(*minishell);
	}
}

int	main(int argc, char **argv, char **base_env)
{
	t_environment		*env;
	t_token				*tokens;
	t_minishell			*minishell;

	(void)argv;
	if (argc > 1)
		return (1);
	collecter_init(&minishell, &env, &tokens);
	get_environment(&env, base_env);
	handle_signals(interactive_sigint, SIG_IGN, SIG_IGN, SIG_IGN);
	minishell_loop(&env, &tokens, &minishell);
	free_environment(env);
	free(g_lists_collecter);
	printf("exit\n");
	return (0);
}
