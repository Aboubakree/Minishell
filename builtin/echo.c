/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 10:41:35 by akrid             #+#    #+#             */
/*   Updated: 2024/06/26 15:39:56 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_result(t_minishell *single_mini, int i)
{
	while (single_mini->args[i])
	{
		printf("%s", single_mini->args[i]);
		if (single_mini->args[i + 1] != NULL)
			printf(" ");
		i++;
	}
}

int	check_valid_new_line(t_minishell *single_mini, int *i)
{
	int	j;

	j = 2;
	while (single_mini->args[*i][j] == 'n')
		j++;
	if (single_mini->args[*i][j] == '\0')
		(*i)++;
	else
		return (1);
	return (0);
}

void	echo(t_minishell *single_mini, t_environment *env)
{
	int	i;
	int	new_line;

	i = 1;
	new_line = 1;
	if (open_files(single_mini, env, single_mini->files))
		return ;
	handel_input_output(single_mini);
	while (single_mini->args[i])
	{
		if (single_mini->args[i][0] == '-' && single_mini->args[i][1] == 'n')
		{
			if (check_valid_new_line(single_mini, &i))
				break ;
			new_line = 0;
		}
		else
			break ;
	}
	print_result(single_mini, i);
	if (new_line == 1)
		printf("\n");
	handel_exit_status(env, single_mini->nbr_cmd, 0);
}
