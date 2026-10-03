#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 100
#define QUANTUM 10

typedef struct {
    char name[10];
    int priority;
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
            tasks[taskCount].priority = priority;
            tasks[taskCount].burst = burst;
            tasks[taskCount].remaining = burst;
            tasks[taskCount].finish = 0;
            tasks[taskCount].done = 0;
            taskCount++;
        }
    }

    fclose(file);
}

int findHighestPriority() {
    int maxPriority = -1;
    for (int i = 0; i < taskCount; i++) {
        if (!tasks[i].done && tasks[i].priority > maxPriority) {
            maxPriority = tasks[i].priority;
        }
    }
    return maxPriority;
}

int hasRemainingTasksInPriority(int prio) {
    for (int i = 0; i < taskCount; i++) {
        if (!tasks[i].done && tasks[i].priority == prio)
            return 1;
    }
    return 0;
}

void scheduleRoundRobinWithPriority() {
    int currentTime = 0;

    printf("Scheduling tasks (Round Robin with Priority, Quantum = %d)...\n\n", QUANTUM);
    printf("%-10s %-10s %-10s %-10s %-10s\n", "Task", "Priority", "RunTime", "Start", "End");
    printf("---------------------------------------------------------------\n");

    while (1) {
        int highestPrio = findHighestPriority();
        if (highestPrio == -1) break;

        while (hasRemainingTasksInPriority(highestPrio)) {
            for (int i = 0; i < taskCount; i++) {
                if (!tasks[i].done && tasks[i].priority == highestPrio) {
                    int runTime = (tasks[i].remaining < QUANTUM) ? tasks[i].remaining : QUANTUM;

                    printf("%-10s %-10d %-10d %-10d %-10d\n",
                           tasks[i].name, tasks[i].priority, runTime, currentTime, currentTime + runTime);

                    currentTime += runTime;
                    tasks[i].remaining -= runTime;

                    if (tasks[i].remaining == 0) {
                        tasks[i].done = 1;
                        tasks[i].finish = currentTime;
                    }
                }
            }
        }
    }

    printf("\n%-10s %-10s %-10s %-14s %-10s\n", "Task", "Priority", "Burst", "Turnaround", "Waiting");
    printf("---------------------------------------------------------------------\n");

    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;

    for (int i = 0; i < taskCount; i++) {
        int turnaround = tasks[i].finish;
        int waiting = turnaround - tasks[i].burst;
        totalWaitingTime += waiting;
        totalTurnaroundTime += turnaround;

        printf("%-10s %-10d %-10d %-14d %-10d\n",
               tasks[i].name, tasks[i].priority, tasks[i].burst,
               turnaround, waiting);
    }

    float avgWaitingTime = (taskCount > 0) ? (float)totalWaitingTime / taskCount : 0.0f;
    float avgTurnaroundTime = (taskCount > 0) ? (float)totalTurnaroundTime / taskCount : 0.0f;

    printf("\nAverage waiting time    = %.2f\n", avgWaitingTime);
    printf("Average turnaround time = %.2f\n", avgTurnaroundTime);
}

int main() {
    loadTasks("task.txt");
    scheduleRoundRobinWithPriority();
    return 0;
}
