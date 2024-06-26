/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 19:40:27 by akrid             #+#    #+#             */
/*   Updated: 2024/06/26 21:05:41 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_directory(char *cmd)
{
	struct stat	path_stat;

	if (stat(cmd, &path_stat) != 0)
		return (0);
	return (S_ISDIR(path_stat.st_mode));
}

int	empty_cmd(char *cmd)
{
	if (cmd == NULL)
	{
		free_at_exit();
		exit(0);
	}
	if (ft_strncmp(cmd, "", 1) == 0 || (ft_strlen(cmd) == 2 && cmd[0] == '"'
			&& cmd[1] == '"'))
	{
		write(2, "'': command not found\n", 22);
		return (free_at_exit(), 1);
	}
	if (ft_strncmp(cmd, ".", 2) == 0)
	{
		write(2, "bash: .: filename argument required\n", 36);
		free_at_exit();
		exit(2);
	}
	if (ft_strncmp(cmd, "..", 3) == 0)
	{
		write(2, "..: command not found\n", 22);
		free_at_exit();
		exit(127);
	}
	return (0);
}
