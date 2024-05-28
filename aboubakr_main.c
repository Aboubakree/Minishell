/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akrid <akrid@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/26 17:02:47 by akrid             #+#    #+#             */
/*   Updated: 2024/05/26 17:04:42 by akrid            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "main.c"
// -------------------------------- start by exe one cmd -------------------------------------------

int args_count(char **args)
{
    int i;

    i = 0;
    while (args && args[i])
        i ++;
    return (i);
}

int    check_builtin(t_minishell *singl_mini, t_environment **env)
{

    if (singl_mini->command == NULL)
        return (1);
    if (ft_strncmp("cd", singl_mini->command, 3) == 0)
        return (cd(singl_mini, *env), 0);
    // if (ft_strncmp("echo", singl_mini->command, 5) == 0)
    //    return (echo(singl_mini, *env), 0);
    if (ft_strncmp("env", singl_mini->command, 4) == 0)
        return (envi(singl_mini, *env), 0);
    if (ft_strncmp("pwd", singl_mini->command, 4) == 0 )
        return (pwd(singl_mini, *env), 0);
    // if (ft_strncmp("export", singl_mini->command, 7) == 0)
    //     return (export(singl_mini, *env), 0);
    if (ft_strncmp("unset", singl_mini->command, 6) == 0)
        return (unset(singl_mini, env), 0);
    if (strncmp("exit", singl_mini->command, 5) == 0)
        return (fake_exit(singl_mini), 0);
    return (1);
}

int cmd_count(t_minishell *minishell)
{
    int count;

    count = 0;
    while (minishell)
    {
        count ++;
        minishell = minishell->next;
    }
    return (count);
}

void fill_heredoc(t_file_redirection *file)
{
    int     fd;
    char    *str;

    fd = open("heredoc_buffer", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
        perror("open");
    while (1)
    {
        str = readline(">");
        if (str == NULL)
        {
            write(2, "bash: warning: here-document delimited by end-of-file\n",
                ft_strlen("bash: warning: here-document delimited by end-of-file\n"));
            break;
        }
        if (ft_strncmp(file->filename, str, ft_strlen(str) + 1) == 0)
        {
            free(str);
            close(fd);
            break;
        }
        write(fd, str, ft_strlen(str));
        write(fd, "\n", 1);
        if(str)
            free(str);
    }
    exit(0);
}

void loop_heredoc(t_file_redirection *files)
{
    while (files)
    {
        if (files->type == T_HERDOC)
            fill_heredoc(files);
        files = files->next;
    }
}

void check_heredoc(t_minishell *minishell)
{
    pid_t   pid;
    t_file_redirection *files;
    int i;

    i = 0;
    files = minishell->files;
    while (files)
    {
        if (files->type == T_HERDOC)
            i = 1;
        files = files->next;
    }
    if (i)
    {
        pid = fork();
        if (pid == 0)
            loop_heredoc(minishell->files);
        wait(NULL);
    }
}

void file_error(char *filename)
{
    write(2, "bash: ",6);
    write(2, filename, ft_strlen(filename));
    write(2, ": ", 2);
    perror("");
    exit(1);
}

void open_files(t_minishell *minishell)
{
    t_file_redirection *files;

    check_heredoc(minishell);
    files = minishell->files;
    while (files)
    {
        if (files->type == T_REDIRECTION_IN)
        {
            if (minishell->infile != 0)
                close(minishell->infile);
            minishell->infile = open(files->filename, O_RDONLY, 0644);
            if (minishell->infile < 0)
                file_error(files->filename);
        }
        else if (files->type == T_REDIRECTION_OUT)
        {
            if (minishell->outfile != 1)
                close(minishell->outfile);
            minishell->outfile = open(files->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (minishell->outfile < 0)
                file_error(files->filename);
        }
        else if (files->type == T_REDIRECTION_APPEND)
        {
            if (minishell->outfile != 1)
                close(minishell->outfile);
            minishell->outfile = open(files->filename, O_WRONLY | O_APPEND | O_CREAT, 0664);
            if (minishell->outfile < 0)
                file_error(files->filename);
        }
        else if (files->type == T_HERDOC)
        {
            if (minishell->infile != 0)
                close(minishell->infile);
            minishell->infile = open("heredoc_buffer", O_RDONLY, 0644);
            if (minishell->infile < 0)
                file_error("heredoc_buffer");
        }
        files = files->next;
    }
}


void    start_execute_one(t_minishell *minishell, t_environment **env)
{
    open_files(minishell);
    if (minishell->command == NULL)
        exit(0);
    minishell->path = get_cmd_path(minishell->command, *env, 0);
    if (minishell->path == NULL)
        exit(127);
    dup2(minishell->infile, 0);
    dup2(minishell->outfile, 1);
    if (execve(minishell->path, minishell->args, NULL))
        perror("execve");
    exit(126);
}

void    execute_one(t_minishell *minishell, t_environment **env)
{
    pid_t   pid;
    int     status;

    pid = fork();
    if (pid == 0)
        start_execute_one(minishell, env);
    wait(&status);
    status = status >> 8;
    //$? = status
    // printf("exit_stat : %d\n", status);
}


// --------------------------------------- end of exe one cmd ----------------------------------------------


//----------------------------------------- multiple commands --------------------------------------


void    pipe_init(t_minishell *mini)
{
    int i;
    int *pip;

    i = 0;
    pip = malloc(sizeof(int) * 2 * (mini->nbr_cmd - 1));
    while (i < mini->nbr_cmd - 1)
    {
        if (pipe(pip + i * 2))
        {
            perror("pipe");
            free_minishell(mini);
            exit(1);
        }
        i ++;
    }
    i = 0;
    while (i < mini->nbr_cmd )
    {
        mini->cmd_order = i;
        mini->pipe = pip;
        i ++;
    }
}

void close_pipes(int *pipe, int nbr_cmd)
{
    int i;

    i = 0;
    while (i < 2 * (nbr_cmd - 1))
        close(pipe[i ++]);
}

void wait_childs(t_minishell *mini, int num_cmd)
{
    int i;
    int status;

    i = 0;
    close_pipes(mini->pipe, mini->nbr_cmd);
    while (i < num_cmd)
    {
        if (wait(&status) > 0)
        {
            status = status >> 8;
            //? = status
            // printf("exit_stat : %d\n", status);
            i ++;
        }
        else
        {
            perror("wait");
            i ++;
        }
    }
}

void get_in_out_priorities(t_minishell *singl_mini)
{
    if (singl_mini->cmd_order == 0)
    {
        dup2(singl_mini->infile, 0);
        if (singl_mini->outfile == 1)
            dup2(singl_mini->pipe[1], 1);
        else
            dup2(singl_mini->outfile, 1);
    }
    else if (singl_mini->cmd_order == singl_mini->nbr_cmd - 1)
    {
        if (singl_mini->infile == 0)
            dup2(singl_mini->pipe[(singl_mini->cmd_order - 1) * 2], 0);
        else
            dup2(singl_mini->infile, 0);
        dup2(singl_mini->outfile, 1);
    }
    else
    {
        if (singl_mini->infile == 0)
            dup2(singl_mini->pipe[(singl_mini->cmd_order - 1) * 2], 0);
        else
            dup2(singl_mini->infile, 0);
        if (singl_mini->outfile == 1)
            dup2(singl_mini->pipe[singl_mini->cmd_order * 2 + 1], 1);
        else
            dup2(singl_mini->outfile, 1);
    }
    close_pipes(singl_mini->pipe, singl_mini->nbr_cmd);
}


void final_execution(t_minishell *singl_mini, t_environment **env)
{
    open_files(singl_mini);
    get_in_out_priorities(singl_mini);
    if (check_builtin(singl_mini, env) == 0)
        exit (0);
    singl_mini->path = get_cmd_path(singl_mini->command, *env, 0);
    if (singl_mini->path == NULL)
        exit(127);
    execve(singl_mini->path, singl_mini->args, NULL);
    perror("execve");
    exit(1);
}

void execute_all(t_minishell *minishell, t_environment **env, int num_cmd)
{
    pid_t       pid;
    t_minishell *temp;
    
    temp = minishell;
    minishell->nbr_cmd = num_cmd;
    pipe_init(minishell);
    while (temp)
    {
        pid = fork();
        if (pid == 0)
            final_execution(temp, env);
        temp = temp->next;
    }
    wait_childs(minishell, num_cmd);
}


//----------------------------------------- end of multiple command --------------------------------


void    execution(t_minishell *minishell, t_environment **env)
{
    if (cmd_count(minishell) == 1)
    {
        if (check_builtin(minishell, env) == 0)
            return ;
        else
            execute_one(minishell, env);
    }
    else
        execute_all(minishell, env, cmd_count(minishell));
}


int main(int argc, char **argv, char **base_env)
{
    t_environment *env;
    t_token *tokens;
    t_minishell *minishell;
    char *str;
    
    (void)argv;
    env = NULL;
    if (argc > 1)
        return (1);
    get_environment(&env, base_env);
    printf("Welcome to minishell\n");
    while (1)
    {
        if (signal(SIGINT, handle_ctrl_c) == SIG_ERR) {
            perror("signal");
            return 1;
        }
        else if (signal(SIGQUIT, SIG_IGN) == SIG_ERR) {
            perror("signal");
            return 1;
        }
        str = readline("\033[0;32mminishell$ \033[0m");
        if (str == NULL)
            break;
        tokens = tokenize_input(str);
        tokens = expand(tokens, env);
        if (check_syntax_error(str) == 1 ||  ft_strlen(str) == 0 || check_syntax_error_tokens(tokens) == 1)
        {
            add_history(str);
            free(str);
            continue;
        }
        minishell = token_to_minishell(tokens);
        minishell = delete_quotes(minishell);
        // print_minishell(minishell);
        execution(minishell, &env);
        if (str[0] != '\0')
            add_history(str);
        free(str);
        free_tokens(tokens);
        free_minishell(minishell);
    }
    free_environment(env);
    return (0);
}