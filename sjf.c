#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 100

typedef struct {
    char name[10];
    int priority;
    int burst;
    int waiting;
    int start;
    int finish;
    int completed;
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
            tasks[taskCount].waiting = 0;
            tasks[taskCount].start = 0;
            tasks[taskCount].finish = 0;
            tasks[taskCount].completed = 0;
            taskCount++;
        }
    }
    fclose(file);
}

int findShortestJob() {
    int minIndex = -1;
    int minBurst = 1e9; 

    for (int i = 0; i < taskCount; i++) {
        if (!tasks[i].completed && tasks[i].burst < minBurst) {
            minBurst = tasks[i].burst;
            minIndex = i;
        }
    }
    return minIndex;
}

void scheduleSJF() {
    int currentTime = 0;
    int completedTasks = 0;
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;

    printf("Scheduling tasks (SJF)...\n\n");
    printf("%-10s %-10s %-10s %-10s %-12s\n", "Task", "Start", "Burst", "Waiting", "Turnaround");
    printf("-------------------------------------------------------------\n");

    while (completedTasks < taskCount) {
        int idx = findShortestJob();
        if (idx == -1) break;

        tasks[idx].start = currentTime;
        tasks[idx].waiting = currentTime;
        tasks[idx].finish = currentTime + tasks[idx].burst;

        int turnaround = tasks[idx].finish;
        totalWaitingTime += tasks[idx].waiting;
        totalTurnaroundTime += turnaround;

        printf("%-10s %-10d %-10d %-10d %-12d\n",
               tasks[idx].name,
               tasks[idx].start,
               tasks[idx].burst,
               tasks[idx].waiting,
               turnaround);

        currentTime += tasks[idx].burst;
        tasks[idx].completed = 1;
        completedTasks++;
    }

    float avgWaitingTime = (taskCount > 0) ? (float)totalWaitingTime / taskCount : 0.0f;
    float avgTurnaroundTime = (taskCount > 0) ? (float)totalTurnaroundTime / taskCount : 0.0f;

    printf("\nAverage waiting time    = %.2f\n", avgWaitingTime);
    printf("Average turnaround time = %.2f\n", avgTurnaroundTime);
}

int main() {
    loadTasks("task.txt");
    scheduleSJF();
    return 0;
}
