#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

// Signal handler for the interactive process
void interactive_sigint_handler(int sig) {
	printf("\n");
    printf("interactive process received SIGINT\n");

}

void interactive_sigquit_handler(int sig) {
	printf("\n");
    printf("interactive process received SIGQUIT\n");
}

// Signal handler for the active process
void active_sigint_handler(int sig) {
    printf("\n");
	printf("active process received SIGINT\n");
	exit(130);
}


void active_sigquit_handler(int sig) {
    printf("\n");
	printf("active process received SIGQUIT SIGINT\n");
	exit(131);
}

void handle_signals(void (*sigint)(int), void (*sigquit)(int))
{
    struct sigaction sa;
	struct sigaction sb;

    // Set up the SIGINT handler for the interactive process
    sa.sa_handler = sigint;
	sb.sa_handler = sigquit;
    sigemptyset(&sa.sa_mask);
	sigemptyset(&sb.sa_mask);
    sa.sa_flags = 0;
	sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sb, NULL);
}

void do_some()
{
	char a;
	while (1)
	{
		scanf("%c", &a);
		if (a == 'a')
			exit(0);
	}
}

int main() {
	char t[50];
	int exit_stat;
	exit_stat = 0;
	pid_t pid;

	handle_signals(&interactive_sigint_handler, &interactive_sigquit_handler);

	while (1)
	{
		printf("say some : ");
		scanf("%s", t);
		if (t[0] == 'q')
			break;
		if (t[0] == 'e')
		{
    		pid = fork();
			if (pid == 0)
			{
				handle_signals(&active_sigint_handler, &active_sigquit_handler);
				do_some();
			}
			wait(&exit_stat);
			// if (WIFEXITED(exit_stat)) 
			// {
			// 	printf("child terminat with exit %d\n", WEXITSTATUS(exit_stat));
			// } 
			// else if (WIFSIGNALED(exit_stat))
			// {
			// 	printf("child terminat with exit %d\n",  128 + WTERMSIG(exit_stat));	
			// 	// if (WTERMSIG(status) == SIGINT) {
			// 	// 	printf("\n");  // Print a newline after ^C
			// 	// }
			// } 
		}
		else 
			printf ("You said : %c\n", t[0]);

		if (WIFEXITED(exit_stat))
		{
			printf("child terminat with exit %d\n", WEXITSTATUS(exit_stat));
		} 
		else if (WIFSIGNALED(exit_stat))
		{
			printf("child terminat with exit %d\n",  128 + WTERMSIG(exit_stat));
		}
		exit_stat = 0;
		t[0] = '\0';
	}
    return 0;
}
