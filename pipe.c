#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pipefd[2];
    pid_t pid;
    char message[] = "Hello from parent!";
    char buffer[100];

    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        return 1;
    }

    // Fork a child process
    pid = fork();

    if (pid < 0) {
        perror("fork failed");
        return 1;
    } else if (pid == 0) {
        // Child process

        // Close write end of the pipe
        close(pipefd[1]);

        // Read message from parent
        read(pipefd[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);

        // Close read end
        close(pipefd[0]);
    } else {
        // Parent process

        // Close read end of the pipe
        close(pipefd[0]);

        // Write message to child
        write(pipefd[1], message, strlen(message) + 1);

        // Close write end
        close(pipefd[1]);

        // Wait for child to finish
        wait(NULL);
    }

    return 0;
}