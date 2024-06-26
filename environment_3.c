/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 13:37:15 by akrid             #+#    #+#             */
/*   Updated: 2024/06/26 17:55:18 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_size(int nbr)
{
	int	size;

	size = 0;
	if (nbr < 0)
		nbr = -nbr;
	while (nbr > 9)
	{
		size++;
		nbr = nbr / 10;
	}
	size++;
	return (size);
}

char	*ft_putnbr(int nbr)
{
	char	*to_str;
	int		nbr_size;
	int		ng;

	ng = 0;
	nbr_size = count_size(nbr);
	if (nbr < 0)
	{
		ng = 1;
		nbr = -nbr;
		nbr_size++;
	}
	to_str = malloc(sizeof(char) * (nbr_size + ng + 1));
	to_str[nbr_size--] = '\0';
	while (nbr_size > 0)
	{
		to_str[nbr_size] = (nbr % 10) + '0';
		nbr = nbr / 10;
		nbr_size--;
	}
	to_str[nbr_size] = (nbr % 10) + '0';
	if (ng)
		to_str[0] = '-';
	return (to_str);
}

void	set_exit_status(t_environment *env, int new_value)
{
	t_environment	*exit_stat;

	exit_stat = env_get_bykey(env, "?");
	if (exit_stat)
	{
		if (exit_stat->value)
			free(exit_stat->value);
		exit_stat->value = ft_putnbr(new_value);
	}
}

t_environment	*creat_env(void)
{
	t_environment	*env;
	char			current_path[1024];

	if (getcwd(current_path, 1024) == NULL)
	{
		perror("getcwd");
		exit(1);
	}
	env = NULL;
	env_add_back(&env, env_node(ft_strdup("PATH"), create_paths()));
	env_add_back(&env, env_node(ft_strdup("PWD"), ft_strdup(current_path)));
	env_add_back(&env, env_node(ft_strdup("SHLVL"), ft_strdup("1")));
	env_add_back(&env, env_node(ft_strdup("?"), ft_strdup("0")));
	env_add_back(&env, env_node(ft_strdup("OLDPWD"), NULL));
	env_add_back(&env, env_node(ft_strdup("_"), ft_strdup("]")));
	lists_collecter->p = 0;
	return (env);
}

void	increment_shlvl(t_environment *env)
{
	t_environment	*shlvl;
	char			*new_lvl;
	int				lvl;

	shlvl = env_get_bykey(env, "SHLVL");
	if (shlvl == NULL || shlvl->value == NULL)
		return ;
	lvl = ft_atoi(shlvl->value);
	lvl++;
	new_lvl = ft_putnbr(lvl);
	free(shlvl->value);
	shlvl->value = new_lvl;
}
