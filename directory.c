#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>  // For file control operations

int main() {
    pid_t pid;
    int pipefd[2];  // Array to hold file descriptors for the pipe

    // Create a pipe: pipefd[0] is the read end, pipefd[1] is the write end
    if (pipe(pipefd) == -1) {
        perror("Pipe failed");
        return 1;
    }

    pid = fork();  // Create a child process

    if (pid < 0) {  // Error in fork()
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0) {  // Child process
        // Close the read end of the pipe, as the child will write to the pipe
        close(pipefd[0]);

        // Redirect stdout to the write end of the pipe
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            perror("dup2 failed");
            return 1;
        }

        // Close the write end of the pipe after redirecting stdout
        close(pipefd[1]);

        // Execute "ls -l" using execlp
        execlp("ls", "ls", "-l", (char *)NULL);

        // If execlp fails
        perror("execlp failed");
        return 1;
    }
    else {  // Parent process
        // Close the write end of the pipe, as the parent will read from the pipe
        close(pipefd[1]);

        // Read the output from the read end of the pipe
        char buffer[1024];
        int n;
        while ((n = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[n] = '\0';  // Null-terminate the string
            printf("%s", buffer);  // Print the output of ls -l
        }

        // Close the read end of the pipe after reading
        close(pipefd[0]);

        // Wait for the child to finish
        wait(NULL);
        printf("\nChild process completed\n");
    }

    return 0;
}