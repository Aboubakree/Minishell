/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_add.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 11:16:04 by akrid             #+#    #+#             */
/*   Updated: 2024/06/26 18:01:40 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_environment	*extract_node(char *arg)
{
	t_environment	*new;
	char			*equal;
	char			*key;
	char			*value;

	equal = ft_strchr(arg, '=');
	if (equal == NULL)
		new = env_node(ft_strdup(arg), NULL);
	else
	{
		key = ft_substr(arg, 0, equal - arg);
		value = ft_substr(equal + 1, 0, ft_strlen(arg));
		new = env_node(key, value);
	}
	return (new);
}

void	parse_error(char *arg)
{
	write(2, "bash: export: ", ft_strlen("bash: export: "));
	write(2, arg, ft_strlen(arg));
	write(2, ": not a valid identifier\n",
		ft_strlen(": not a valid identifier\n"));
}

int	pars_node(t_environment *new, char *arg)
{
	int	i;
	int	plus;

	i = 0;
	plus = 0;
	while (new && new->key && new->key[i])
	{
		if ((new->key[i] < 'a' || new->key[i] > 'z') && (new->key[i] < 'A'
				|| new->key[i] > 'Z') && (new->key[i] < '0'
				|| new->key[i] > '9') && new->key[i] != '_'
			&& new->key[i] != '+')
			return (parse_error(arg), 1);
		if (new->key[i] == '+')
			plus++;
		if (plus > 1 || new->key[0] == '+' || (new->key[0] >= '0'
				&& new->key[0] <= '9'))
			return (parse_error(arg), 1);
		i++;
	}
	if ((new->key && new->key[0] == '\0') || (new->key[ft_strlen(new->key)
				- 1] == '+' && new->value == NULL) || (plus == 1
			&& new->key[ft_strlen(new->key) - 1] != '+'))
		return (parse_error(arg), 1);
	return (0);
}

void	append_value(t_environment *env, t_environment *new)
{
	t_environment	*target;
	t_environment	*temp;
	t_environment	*previous;
	char			*helper;

	new->key[ft_strlen(new->key) - 1] = '\0';
	if (ft_strncmp(new->key, "PATH", 5) == 0)
		lists_collecter->p = 1;
	target = env_get_bykey(env, new->key);
	temp = env->next->next;
	previous = env->next;
	if (target == NULL)
	{
		previous->next = new;
		new->next = temp;
	}
	else if (new->value != NULL)
	{
		helper = ft_strjoin(target->value, new->value);
		free(target->value);
		target->value = helper;
		free_environment(new);
	}
	else
		free_environment(new);
}

void	replace_value(t_environment *env, t_environment *new)
{
	t_environment	*target;
	t_environment	*temp;
	t_environment	*previous;

	target = env_get_bykey(env, new->key);
	temp = env;
	previous = (env)->next;
	if (ft_strncmp(new->key, "PATH", 5) == 0)
		lists_collecter->p = 1;
	if (target == NULL)
	{
		temp = temp->next->next;
		previous->next = new;
		new->next = temp;
	}
	else if (new->value != NULL)
	{
		free(target->value);
		target->value = ft_strdup(new->value);
		free_environment(new);
	}
	else
		free_environment(new);
}
