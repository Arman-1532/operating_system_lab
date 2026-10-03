#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>  // for wait()

int main()
{
    pid_t pid;

    /* fork a child process */
    pid = fork();

    if (pid < 0) { /* error occurred */
        fprintf(stderr, "Fork Failed\n");
        return 1;
    }
    else if (pid == 0) { /* child process */
        if (execlp("/bin/ls", "ls", NULL) == -1) {
            // If execlp fails
            perror("execlp failed");
            return 1;
        }
    }
    else { /* parent process */
        /* parent will wait for the child to complete */
        wait(NULL);
        printf("Child Complete\n");
    }

    return 0;
}
