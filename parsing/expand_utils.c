/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: rtamouss <rtamouss@student.42.fr>            +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/06/29 10:24:27 by rtamouss      #+#    #+#                 */
/*   Updated: 2024/06/30 17:28:32 by rtamouss      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*change_value(char *str, int *i, int *index, char *env_variable)
{
	char			*new_value;
	t_environment	*get_env;
	char			*value;
	char			*first;
	char			*last;

	env_variable = ft_substr(str, *i + 1, *index - *i - 1);
	get_env = env_get_bykey(*(lists_collecter->env), env_variable);
	if (get_env == NULL)
		value = ft_strdup("");
	else
		value = ft_strdup(get_env->value);
	first = ft_substr(str, 0, *i);
	last = ft_strdup(&str[*index]);
	new_value = ft_strjoin(first, value);
	free(value);
	value = new_value;
	new_value = ft_strjoin(value, last);
	free(value);
	free(last);
	free(first);
	free(env_variable);
	free(str);
	return (new_value);
}

int	expand_helper(int index, int *i, char *str)
{
	index = *i;
	index++;
	if (str[index] == '?' || is_number(str[index]))
		index++;
	else
	{
		while (str[index] && is_word(str[index]))
			index++;
	}
	return (index);
}

int	not_alpha_numeric(char c)
{
	return (!is_alpha(c) && !is_number(c) && c != '_' && c != '?' && c != '`');
}
