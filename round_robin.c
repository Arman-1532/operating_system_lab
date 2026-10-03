#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 100
#define QUANTUM 10

typedef struct {
    char name[10];
    int burst;
    int remaining;
    int finish;
    int done;
} Task;

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
            tasks[taskCount].burst = burst;
            tasks[taskCount].remaining = burst;
            tasks[taskCount].done = 0;
            tasks[taskCount].finish = 0;
            taskCount++;
        }
    }

    fclose(file);
}

void scheduleRoundRobin() {
    int currentTime = 0;
    int completed = 0;

    printf("Scheduling tasks (Round Robin, Quantum = %d)...\n\n", QUANTUM);
    printf("%-10s %-10s %-10s %-10s\n", "Task", "RunTime", "Start", "End");
    printf("--------------------------------------------------\n");

    while (completed < taskCount) {
        for (int i = 0; i < taskCount; i++) {
            if (tasks[i].remaining > 0) {
                int runTime = (tasks[i].remaining < QUANTUM) ? tasks[i].remaining : QUANTUM;

                printf("%-10s %-10d %-10d %-10d\n",
                       tasks[i].name, runTime, currentTime, currentTime + runTime);

                currentTime += runTime;
                tasks[i].remaining -= runTime;

                if (tasks[i].remaining == 0) {
                    tasks[i].done = 1;
                    tasks[i].finish = currentTime;
                    completed++;
                }
            }
        }
    }

    printf("\n%-10s %-10s %-12s %-10s\n", "Task", "Burst", "Turnaround", "Waiting");
    printf("--------------------------------------------------\n");

    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;

    for (int i = 0; i < taskCount; i++) {
        int turnaround = tasks[i].finish;
        int waiting = turnaround - tasks[i].burst;
        totalWaitingTime += waiting;
        totalTurnaroundTime += turnaround;

        printf("%-10s %-10d %-12d %-10d\n",
               tasks[i].name, tasks[i].burst, turnaround, waiting);
    }

    float avgWaitingTime = (float)totalWaitingTime / taskCount;
    float avgTurnaroundTime = (float)totalTurnaroundTime / taskCount;

    printf("\nAverage waiting time    = %.2f\n", avgWaitingTime);
    printf("Average turnaround time = %.2f\n", avgTurnaroundTime);
}

int main() {
    loadTasks("task.txt");
    scheduleRoundRobin();
    return 0;
}
