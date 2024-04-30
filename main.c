/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 10:18:34 by akrid             #+#    #+#             */
/*   Updated: 2024/04/30 13:14:36 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_cmd_paths(char **all_paths)
{
    int i;

    i = 0;
    while (all_paths && all_paths[i])
        free(all_paths[i ++]);
    if (all_paths)
        free(all_paths);
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
			return (free_cmd_paths(all_paths), path);
		free(path);
		i++;
	}
	if (all_paths)
		free_cmd_paths(all_paths);
	return (NULL);
}

int main(int argc, char **argv, char **base_env)
{
    t_environment *env;
    // t_environment *OLDPWD;
    // t_environment *PWD;
    char *path;
    char *cc[2];
    
    (void)argv;
    env = NULL;
    cc[1] = NULL;
    if (argc > 1)
        return (1);
    get_environment(&env, base_env);
    char *str;
    while (1)
    {
        str = readline("minishell-->$");
        if (str == NULL)
            break;
        path = get_cmd_path(str, env);
        if (path == NULL)
            printf("%s : command not found\n", str);
        else
        {
            cc[0] = str;
            execve(path, &str, NULL);
        } 
        // if (ft_strncmp("cd", str, 2) == 0)
        // {
        //     cd(str + 3, env);
        //     OLDPWD = env_get_bykey(env, "OLDPWD");
        //     PWD = env_get_bykey(env, "SHELL");
        //     // printf("%s=%s\n", OLDPWD->key, OLDPWD->value);
        //     printf("%s=%s\n", PWD->key, PWD->value);
        // }
        // else if (ft_strncmp("pwd", str, 3) == 0)
        //     pwd(env);
        // else if (ft_strncmp("env", str, 3) == 0)
        //     envi(env);
        // else
        //     printf("Your line : %s\n", str);
        free(str);
    }
    return (0);
}
