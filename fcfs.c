#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[10];
    int priority;
    int burst;
} Task;

#define MAX_TASKS 100

Task tasks[MAX_TASKS];
int taskCount = 0;

void loadTasks(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Cannot open file %s\n", filename);
        exit(1);
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char name[10];
        int priority, burst;
        if (sscanf(line, "%[^,], %d, %d", name, &priority, &burst) == 3) {
            strcpy(tasks[taskCount].name, name);
            tasks[taskCount].priority = priority;
            tasks[taskCount].burst = burst;
            taskCount++;
        }
    }

    fclose(file);
}

void scheduleTasks() {
    printf("Scheduling tasks (FCFS)...\n\n");

    printf("%-10s %-10s %-10s %-12s %-10s\n", "Task", "Burst", "Start", "Turnaround", "Waiting");
    printf("---------------------------------------------------------------\n");

    int currentTime = 0;
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;

    for (int i = 0; i < taskCount; i++) {
        int start = currentTime;
        int turnaround = start + tasks[i].burst;
        int waiting = start;

        printf("%-10s %-10d %-10d %-12d %-10d\n",
               tasks[i].name, tasks[i].burst, start, turnaround, waiting);

        totalWaitingTime += waiting;
        totalTurnaroundTime += turnaround;
        currentTime = turnaround;
    }

    float avgWaitingTime = (taskCount > 0) ? (float)totalWaitingTime / taskCount : 0.0f;
    float avgTurnaroundTime = (taskCount > 0) ? (float)totalTurnaroundTime / taskCount : 0.0f;

    printf("\nAverage waiting time    = %.2f\n", avgWaitingTime);
    printf("Average turnaround time = %.2f\n", avgTurnaroundTime);
}

int main() {
    loadTasks("task.txt");
    scheduleTasks();
    return 0;
}
