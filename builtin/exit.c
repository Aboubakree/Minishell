/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/02 09:11:41 by akrid             #+#    #+#             */
/*   Updated: 2024/07/01 20:29:09 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_digits(char *str)
{
	int	i;

	i = 0;
	if (str && (str[i] == '-' || str[i] == '+'))
		i++;
	if (str && str[i] == '\0')
		return (0);
	while (str && str[i])
	{
		if (ft_isdigit(str[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

void	print_error(char *arg, int flag)
{
	if (flag == 0)
	{
		write(2, "bash: exit: ", ft_strlen("bash: exit: "));
		write(2, arg, ft_strlen(arg));
		write(2, ": numeric argument required\n",
			ft_strlen(": numeric argument required\n"));
		free_at_exit();
		exit(2);
	}
	if (flag == 1)
	{
		write(2, "bash: exit: too many arguments\n",
			ft_strlen("bash: exit: too many arguments\n"));
	}
}

void	fake_exit(t_minishell *singl_mini, t_environment *env)
{
	int	exit_status;

	if (open_files(singl_mini, env, singl_mini->files))
		return ;
	handel_input_output(singl_mini);
	exit_status = ft_atoi((env_get_bykey(env, "?")->value));
	if (singl_mini->nbr_cmd == 1)
		printf("exit\n");
	if (singl_mini->args && singl_mini->args[1])
	{
		if (check_digits(singl_mini->args[1]) == 0)
			print_error(singl_mini->args[1], 0);
		if (singl_mini->args[2] != NULL)
		{
			set_exit_status(env, 1);
			return (print_error(singl_mini->args[1], 1), (void)0);
		}
		exit_status = ft_atoi(singl_mini->args[1]);
	}
	free_at_exit();
	exit(exit_status);
}
