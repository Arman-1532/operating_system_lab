#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int pipefd1[2], pipefd2[2];  // Two pipes: one for parent->child, one for child->parent
    pid_t pid;
    char parent_message[] = "Hello from parent!";
    char child_message[] = "Hello from child!";
    char buffer[100];

    // Create two pipes
    if (pipe(pipefd1) == -1 || pipe(pipefd2) == -1) {
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

        // Close write end of pipefd1 (parent -> child) and read end of pipefd2 (child -> parent)
        close(pipefd1[1]);
        close(pipefd2[0]);

        // Read message from parent
        read(pipefd1[0], buffer, sizeof(buffer));
        printf("Child received: %s\n", buffer);

        // Send message to parent
        write(pipefd2[1], child_message, strlen(child_message) + 1);

        // Close both ends of pipes
        close(pipefd1[0]);
        close(pipefd2[1]);
    } else {
        // Parent process

        // Close read end of pipefd1 (parent -> child) and write end of pipefd2 (child -> parent)
        close(pipefd1[0]);
        close(pipefd2[1]);

        // Send message to child
        write(pipefd1[1], parent_message, strlen(parent_message) + 1);

        // Read message from child
        read(pipefd2[0], buffer, sizeof(buffer));
        printf("Parent received: %s\n", buffer);

        // Close both ends of pipes
        close(pipefd1[1]);
        close(pipefd2[0]);

        // Wait for child to finish
        wait(NULL);
    }

    return 0;
}