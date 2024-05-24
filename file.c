#include <stdio.h>
#include <unistd.h>

int main() {
    int pipe_fds[2];  // Array to store pipe file descriptors (read end, write end)

    // Create the pipe
    if (pipe(pipe_fds) == -1) {
        perror("pipe");
        return 1;
    }

    // Redirect stdin to the read end of the pipe
    if (dup2(pipe_fds[0], STDIN_FILENO) == -1) {
        perror("dup2 (stdin)");
        return 1;
    }

    // Redirect stdout to the write end of the pipe
    if (dup2(pipe_fds[1], STDOUT_FILENO) == -1) {
        perror("dup2 (stdout)");
        return 1;
    }

    // Now, any data written to "stdout" will be written to the pipe,
    // and any data read from "stdin" will be read from the pipe.

    // Example: Write some data to the pipe (which is now our "stdout")
    printf("This message is written to the pipe (redirected stdout)\n");

    // You can now use this redirected input/output with other processes
    // or for further processing within your program.

    // Close the pipe's file descriptors (not strictly necessary here)
    close(pipe_fds[0]);
    close(pipe_fds[1]);

    return 0;
}
