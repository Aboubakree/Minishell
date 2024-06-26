/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 11:13:35 by akrid             #+#    #+#             */
/*   Updated: 2024/06/26 18:05:06 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	swap_data(t_environment *helper)
{
	char	*key_temp;
	char	*value_temp;

	key_temp = helper->key;
	helper->key = helper->next->key;
	helper->next->key = key_temp;
	value_temp = helper->value;
	helper->value = helper->next->value;
	helper->next->value = value_temp;
}

void	sort(t_environment **sort_env)
{
	int				b;
	t_environment	*helper;

	b = 1;
	while (b)
	{
		b = 0;
		helper = *sort_env;
		while (helper && helper->next)
		{
			if (strncmp(helper->key, helper->next->key, ft_strlen(helper->key)
					+ 1) > 0)
			{
				swap_data(helper);
				b = 1;
			}
			helper = helper->next;
		}
	}
}

t_environment	*duplicate_env(t_environment *env)
{
	t_environment	*copy;

	copy = NULL;
	while (env)
	{
		if (env->value == NULL)
			env_add_back(&copy, env_node(ft_strdup(env->key), NULL));
		else
			env_add_back(&copy, env_node(ft_strdup(env->key),
					ft_strdup(env->value)));
		env = env->next;
	}
	return (copy);
}

void	free_copy(t_environment *env)
{
	t_environment	*temp;

	temp = env;
	while (env)
	{
		temp = env;
		env = env->next;
		free(temp->key);
		free(temp->value);
		free(temp);
	}
}

void	print(char *key, char *value)
{
	if (value != NULL)
		printf("declare -x %s=\"%s\"\n", key, value);
	else
		printf("declare -x %s\n", key);
}
