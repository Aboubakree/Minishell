/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtamouss <rtamouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 16:19:23 by rtamouss          #+#    #+#             */
/*   Updated: 2024/06/29 19:24:03 by rtamouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	in_quote(const char *str)
{
	size_t	len;

	len = ft_strlen(str);
	return (len >= 2 && str[0] == '"' && str[len - 1] == '"');
}

int	is_empty(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (is_whitespace(str[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

int	check_delemeter_for_sr(char *str, char *filename)
{
	if (str == NULL)
	{
		write(2, "bash: warning: here-document delimited by end-of-file\n",
			ft_strlen(\
				"bash: warning: here-document delimited by end-of-file\n"));
		return (1);
	}
	if (ft_strncmp(filename, str, ft_strlen(str) + 1) == 0)
	{
		free(str);
		return (1);
	}
	return (0);
}

void	print_heredocs(t_file_redirection *files)
{
	t_file_redirection	*temp;

	temp = files;
	while (temp)
	{
		printf("filename = %s [%s]\n",
			temp->filename, get_type_token(temp->type));
		temp = temp->next;
	}
}

void	collecter_init(t_minishell **minishell,
	t_environment **env, t_token **tokens)
{
	*env = NULL;
	*tokens = NULL;
	*minishell = NULL;
	lists_collecter = malloc(sizeof(t_lists_collecter));
	if (lists_collecter == NULL)
	{
		perror("malloc");
		exit(1);
	}
	lists_collecter->minishell = minishell;
	lists_collecter->env = env;
	lists_collecter->tokens = tokens;
	lists_collecter->p = 1;
}
