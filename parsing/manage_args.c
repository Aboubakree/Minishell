/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtamouss <rtamouss@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 21:29:21 by rtamouss          #+#    #+#             */
/*   Updated: 2024/06/29 19:09:09 by rtamouss         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	*new_arg(char *arg)
{
	t_args	*args;

	args = (t_args *)malloc(sizeof(t_args));
	if (!args)
		return (NULL);
	args->args = arg;
	args->next = NULL;
	return (args);
}

void	add_arg_back(t_args **head, t_args *new_arg)
{
	t_args	*temp;

	if (!*head)
		*head = new_arg;
	else
	{
		temp = *head;
		while (temp->next)
			temp = temp->next;
		temp->next = new_arg;
	}
}

int	count_args(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

void	join_args(t_token *temp, char **args1, char ***args)
{
	char	*temp_args1;

	temp_args1 = ft_strjoin(*args1, temp->value);
	free(*args1);
	*args1 = temp_args1;
	temp_args1 = ft_strjoin(*args1, "\r");
	free(*args1);
	*args1 = temp_args1;
	free_args(*args);
	*args = ft_split2(*args1, '\r');
}

void	print_args(t_minishell *temp)
{
	int	i;

	i = 0;
	if (temp->args)
	{
		printf("args: ");
		while (temp->args[i])
		{
			printf("[%s]", temp->args[i]);
			i++;
		}
	}
	printf("\n");
}
