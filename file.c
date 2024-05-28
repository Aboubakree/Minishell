#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

// Function to find the full path of a command
char *find_command(char *cmd) {
    char *path = getenv("PATH");
    char *token = strtok(path, ":");
    char full_path[256];

    while (token != NULL) {
        snprintf(full_path, sizeof(full_path), "%s/%s", token, cmd);
        if (access(full_path, X_OK) == 0) {
            return strdup(full_path);
        }
        token = strtok(NULL, ":");
    }
    return NULL;
}

// Function to execute a command
void execute_command(char *cmd, char **envp) {
    char *args[100];
    int i = 0;
    args[i] = strtok(cmd, " ");
    while (args[i] != NULL) {
        args[++i] = strtok(NULL, " ");
    }

    char *cmd_path = find_command(args[0]);
    if (cmd_path != NULL) {
        execve(cmd_path, args, envp);
        perror("execve");
        free(cmd_path);
    } else {
        fprintf(stderr, "Command not found: %s\n", args[0]);
        exit(EXIT_FAILURE);
    }
}

// Function to execute a pipeline of commands
void execute_pipeline(char **cmds, int num_cmds, char **envp) {
    int i;
    int pipefds[2 * (num_cmds - 1)];

    // Create the pipes
    for (i = 0; i < num_cmds - 1; i++) {
        if (pipe(pipefds + i * 2) < 0) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    int pid;
    for (i = 0; i < num_cmds; i++) {
        pid = fork();
        if (pid == 0) {
            // Redirect input from the previous command if it's not the first command
            if (i != 0) {
                if (dup2(pipefds[(i - 1) * 2], 0) < 0) {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
            }
            // Redirect output to the next command if it's not the last command
            if (i != num_cmds - 1) {
                if (dup2(pipefds[i * 2 + 1], 1) < 0) {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
            }

            // Close all pipe file descriptors in the child process
            for (int j = 0; j < 2 * (num_cmds - 1); j++) {
                close(pipefds[j]);
            }

            execute_command(cmds[i], envp);
        } else if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    // Close all pipe file descriptors in the parent process
    for (int j = 0; j < 2 * (num_cmds - 1); j++) {
        close(pipefds[j]);
    }

    // Wait for all child processes to finish
    for (i = 0; i < num_cmds; i++) {
        wait(NULL);
    }
}

int main(int argc, char **argv, char **envp) {
    char *str;

    (void)argc;
    (void)argv;
    (void)envp;
    printf("Welcome to minishell\n");

    while (1) {
        str = readline("minishell$ ");
        if (str == NULL) {
            break;
        }

        if (str[0] != '\0') {
            add_history(str);

            // Split the input line by '|' to get individual commands
            char *cmds[100];
            int num_cmds = 0;
            cmds[num_cmds] = strtok(str, "|");
            while (cmds[num_cmds] != NULL) {
                cmds[++num_cmds] = strtok(NULL, "|");
            }

            execute_pipeline(cmds, num_cmds, envp);
        }
        free(str);
    }

    return 0;
}
