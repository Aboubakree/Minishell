/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 10:47:20 by akrid             #+#    #+#             */
/*   Updated: 2024/05/13 10:31:11 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_split(char **splited)
{
    int i;

    i = 0;
    while (splited && splited[i])
        free(splited[i ++]);
    if (splited)
        free(splited);
}

char	*get_cmd_path(char *cmd, t_environment *env)
{
	int		i;
	char	**all_paths;
	char	*path;
	char	*temp;

	all_paths = ft_split((env_get_bykey(env, "PATH"))->value, ':');
	i = 0;
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
	if (all_paths)
		free_split(all_paths);
	printf("%s: command not found\n", cmd);
	// ? = 127;
	return (NULL);
}

void path(t_minishell *mini, t_environment *env)
{
	while (mini)
	{
		mini->path = get_cmd_path(mini->command, env);
		mini = mini->next;
	}
}