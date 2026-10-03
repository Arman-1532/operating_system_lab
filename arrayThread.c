#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 4 

typedef struct {
    int *array;
    int start;
    int end;
    long partial_sum;  
} ThreadData;

void *sum_array(void *param) {
    ThreadData *data = (ThreadData *)param;
    data->partial_sum = 0;
    for (int i = data->start; i < data->end; i++) {
        data->partial_sum += data->array[i];
    }
    pthread_exit(0);
}

int main() {
    int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,17,18,19,20};
    int length = sizeof(array) / sizeof(array[0]);

    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    int chunk_size = length / NUM_THREADS;
    int i;

    for (i = 0; i < NUM_THREADS; i++) {
        thread_data[i].array = array;
        thread_data[i].start = i * chunk_size;

        thread_data[i].end = (i == NUM_THREADS - 1) ? length : (i + 1) * chunk_size;
        thread_data[i].partial_sum = 0;

        pthread_create(&threads[i], NULL, sum_array, (void *)&thread_data[i]);
    }

    long total_sum = 0;
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        total_sum += thread_data[i].partial_sum;
    }

    printf("Total sum of array elements = %ld\n", total_sum);
    return 0;
}