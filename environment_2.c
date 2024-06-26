/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 11:34:29 by akrid             #+#    #+#             */
/*   Updated: 2024/06/23 13:29:53 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_size(t_environment *env)
{
	int				i;
	t_environment	*temp;

	i = 0;
	temp = env;
	while (temp)
	{
		temp = temp->next;
		i++;
	}
	return (i);
}

char	**convert_env(t_environment *env)
{
	char	**env_convert;
	char	*temp;
	int		i;

	i = 0;
	env_convert = NULL;
	env_convert = malloc(sizeof(char *) * (env_size(env) + 1));
	while (env)
	{
		temp = ft_strjoin(env->key, "=");
		env_convert[i] = ft_strjoin(temp, env->value);
		free(temp);
		i++;
		env = env->next;
	}
	env_convert[i] = NULL;
	return (env_convert);
}

t_environment	*env_get_last(t_environment *env)
{
	while (env && env->next)
		env = env->next;
	return (env);
}

void	put_at_end(t_environment **env, char *target_key)
{
	t_environment	*target;
	t_environment	*previuos;
	t_environment	*last;

	target = env_get_bykey(*env, target_key);
	previuos = get_before_target(*env, target);
	last = env_get_last(*env);
	if (target == NULL || target == last || env_size(*env) < 2)
		return ;
	if (target == *env)
		*env = (*env)->next;
	else
		previuos->next = target->next;
	target->next = NULL;
	last->next = target;
}

char	*create_paths(void)
{
	char	*p;

	p = ft_strdup(
			"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/snap/bin"
			);
	return (p);
}
