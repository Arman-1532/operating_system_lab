#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 5  // Size of the circular buffer (number of messages)
#define MESSAGE_SIZE 100  // Size of each message

// Circular buffer structure
typedef struct {
    char buffer[BUFFER_SIZE][MESSAGE_SIZE];
    int head;
    int tail;
} CircularBuffer;

// Function to initialize the buffer
void initBuffer(CircularBuffer *cb) {
    cb->head = 0;
    cb->tail = 0;
}

// Function to write to the circular buffer
void writeToBuffer(CircularBuffer *cb, const char *message) {
    // Copy message to the buffer
    strncpy(cb->buffer[cb->tail], message, MESSAGE_SIZE);
    // Move the tail to the next position
    cb->tail = (cb->tail + 1) % BUFFER_SIZE;
    // If the buffer is full, move the head
    if (cb->tail == cb->head) {
        cb->head = (cb->head + 1) % BUFFER_SIZE;  // Overwrite the oldest message
    }
}

// Function to read from the circular buffer
void readFromBuffer(CircularBuffer *cb, char *message) {
    // Copy the message from the head of the buffer
    strncpy(message, cb->buffer[cb->head], MESSAGE_SIZE);
    // Move the head to the next position
    cb->head = (cb->head + 1) % BUFFER_SIZE;
}

int main() {
    int pipefd1[2], pipefd2[2];  // Two pipes: one for parent->child, one for child->parent
    pid_t pid;
    CircularBuffer cb1, cb2;  // Circular buffers for parent->child and child->parent
    char parent_message[] = "Hello from parent!";
    char child_message[] = "Hello from child!";
    char buffer[MESSAGE_SIZE];

    // Initialize circular buffers
    initBuffer(&cb1);
    initBuffer(&cb2);

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

        // Simulate circular buffer usage: Read from parent and write to child
        read(pipefd1[0], buffer, sizeof(buffer));
        writeToBuffer(&cb1, buffer);  // Write received message to the circular buffer
        printf("Child received and stored in buffer: %s\n", buffer);

        // Send message to parent
        writeToBuffer(&cb2, child_message);
        write(pipefd2[1], child_message, strlen(child_message) + 1);

        // Close both ends of pipes
        close(pipefd1[0]);
        close(pipefd2[1]);
    } else {
        // Parent process

        // Close read end of pipefd1 (parent -> child) and write end of pipefd2 (child -> parent)
        close(pipefd1[0]);
        close(pipefd2[1]);

        // Write to circular buffer
        writeToBuffer(&cb1, parent_message);
        // Send message to child via pipe
        write(pipefd1[1], parent_message, strlen(parent_message) + 1);

        // Read from child (using circular buffer simulation)
        read(pipefd2[0], buffer, sizeof(buffer));
        writeToBuffer(&cb2, buffer);  // Store child message in circular buffer
        printf("Parent received and stored in buffer: %s\n", buffer);

        // Close both ends of pipes
        close(pipefd1[1]);
        close(pipefd2[0]);

        // Wait for child to finish
        wait(NULL);
    }

    return 0;
}