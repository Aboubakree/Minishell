/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   free_data2.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rtamouss <rtamouss@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/07/01 21:51:54 by rtamouss      #+#    #+#                 */
/*   Updated: 2024/07/01 21:51:55 by rtamouss      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_free(char **strs)
{
	int	i;

	i = 0;
	while (strs[i])
		free(strs[i++]);
	free(strs);
	return (0);
}

void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current != NULL)
	{
		next = current->next;
		if (current->value != NULL)
		{
			free(current->value);
			current->value = NULL;
		}
		free(current);
		current = next;
	}
}

void	free_args(char **args)
{
	int	i;

	i = 0;
	if (args)
	{
		while (args[i])
		{
			free(args[i]);
			i++;
		}
		free(args);
	}
}
