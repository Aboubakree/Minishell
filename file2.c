#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

// Signal handler for the parent process
void interactive_sigint(int sig) {
	(void)sig;
	// printf("\n");
	// rl_on_new_line();
	// rl_replace_line("", 1);
	// rl_redisplay();
	// set_exit_status(*(lists_collecter->env), 130);
    printf("Parent process received SIGINT\n");
}

void interactive_sigquit(int sig) {
	(void)sig;
    printf("Parent process received SIGQUIT\n");
}

// Signal handler for the child process
void active_sigint(int sig) {
	(void)sig;
    write(2, "\n", 1);
	exit(130);
}

void active_sigquit(int sig) {
	(void)sig;
    write(2,"Quit (core dumped)\n", 20);
	exit(131);
}

void handle_signals(void (*sigint)(int), void (*sigquit)(int), void (*sigint_old)(int), void (*sigquit_old)(int)) 
{
    struct sigaction s_int;
	struct sigaction s_int_old;
    struct sigaction s_quit;
	struct sigaction s_quit_old;

    // Set up the SIGINT handler
    s_int.sa_handler = sigint;
    sigemptyset(&s_int.sa_mask);
    s_int.sa_flags = 0;
    s_int_old.sa_handler = sigint_old;
    sigemptyset(&s_int_old.sa_mask);
    s_int_old.sa_flags = 0;
    sigaction(SIGINT, &s_int, &s_int_old);
    // Set up the SIGQUIT handler
    s_quit.sa_handler = sigquit;
    sigemptyset(&s_quit.sa_mask);
    s_quit.sa_flags = 0;
    s_quit_old.sa_handler = sigquit_old;
    sigemptyset(&s_quit_old.sa_mask);
    s_quit_old.sa_flags = 0;
    sigaction(SIGQUIT, &s_quit, &s_quit_old);
}

int main() {
    char t[50];
    // Set up the ignore ction for SIGINT and SIGQUIT

    handle_signals(interactive_sigint, interactive_sigquit,  SIG_IGN,  SIG_IGN);
    // Save current handlers and set to ignore in parent
	handle_signals(SIG_IGN, SIG_IGN,  interactive_sigint,   interactive_sigquit);

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Restore default signal handling in child
        handle_signals(active_sigint, active_sigquit, SIG_IGN, SIG_IGN);

        // Keep the child process running to test signal handling
        printf("Child process running with PID %d\n", getpid());
        while (1) {
            printf("to quit child press q: ");
            scanf("%s", t);
            if (t[0] == 'q')
                break;
            printf("You said in child: %s\n", t); // Wait for signals
        }
        exit(0); // Ensure child exits after breaking out of the loop
    } else {
        // Parent process
        printf("Parent process running with PID %d\n", getpid());

        int status;
        // Wait for child process to finish and get exit status
        waitpid(pid, &status, 0);

        // Check if the child exited normally or was terminated by a signal
        if (WIFEXITED(status)) {
            printf("Child exited with status %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Child terminated by signal %d\n", WTERMSIG(status));
        }

        // Restore original signal handlers in parent
		handle_signals(interactive_sigint, interactive_sigquit, SIG_IGN, SIG_IGN);

        // Keep the parent process running to test signal handling
        while (1) {
            printf("say some: ");
            scanf("%s", t);
            if (t[0] == 'q')
                break;
            printf("You said: %s\n", t); // Wait for signals
        }
    }

    return 0;
}
