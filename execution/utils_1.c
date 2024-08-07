/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 19:40:15 by akrid             #+#    #+#             */
/*   Updated: 2024/06/26 21:01:11 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	parse_cmds(char *command)
{
	if (command && ft_strchr(command, '/') != NULL)
	{
		if (is_directory(command))
		{
			write(2, "bash: ", 6);
			write(2, command, ft_strlen(command));
			write(2, ": Is a directory\n", 17);
			free_at_exit();
			exit(126);
		}
		if (access(command, F_OK) == 0)
			return (1);
		else
		{
			write(2, "bash: ", 6);
			write(2, command, ft_strlen(command));
			write(2, ": ", 2);
			perror("");
			free_at_exit();
			exit(127);
		}
	}
	if (empty_cmd(command))
		exit(127);
	return (0);
}

void	free_split(char **splited)
{
	int	i;

	i = 0;
	while (splited && splited[i])
		free(splited[i++]);
	if (splited)
		free(splited);
}

char	**check_paths(t_environment *temp, char *cmd)
{
	if (temp == NULL || (temp->value == NULL || ft_strncmp(temp->value, "",
				1) == 0))
	{
		write(2, "bash: ", 6);
		write(2, cmd, ft_strlen(cmd));
		write(2, ": No such file or directory\n",
			ft_strlen(": No such file or directory\n"));
		return (NULL);
	}
	return (ft_split(temp->value, ':'));
}

char	*get_cmd_path(char *cmd, t_environment *env, int i)
{
	char	**all_paths;
	char	*path;
	char	*temp;

	if (parse_cmds(cmd))
		return (ft_strdup(cmd));
	all_paths = check_paths(env_get_bykey(env, "PATH"), cmd);
	if (all_paths == NULL)
		return (NULL);
	while (all_paths && all_paths[i])
	{
		temp = ft_strjoin(all_paths[i], "/");
		path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(path, F_OK) == 0)
			return (free_split(all_paths), path);
		free(path);
		i++;
	}
	free_split(all_paths);
	write(2, cmd, ft_strlen(cmd));
	write(2, ": command not found\n", ft_strlen(": command not found\n"));
	return (NULL);
}
