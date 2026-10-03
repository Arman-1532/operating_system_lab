#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0, out = 0;
int counter = 0;

int next_produced = 1;
int next_consumed;

void producer(int pro) {
    if (counter == BUFFER_SIZE) {
        printf("Buffer is full. Cannot produce.\n");
        return;
    }

    buffer[in] = next_produced;
    printf("Produced: %d at index %d\n", pro, in);
    next_produced++;
    in = (in + 1) % BUFFER_SIZE;
    counter++;
}

void consumer() {
    if (counter == 0) {
        printf("Buffer is empty. Cannot consume.\n");
        return;
    }

    next_consumed = buffer[out];
    printf("Consumed: %d from index %d\n", next_consumed, out);
    out = (out + 1) % BUFFER_SIZE;
    counter--;
}

int main() {
    int choice;

    producer(11);
    producer(12);
    consumer();
    producer(13);
    producer(14);
    producer(15);
    
    producer(16);
    producer(17);
    consumer();
    consumer();
    consumer();
    consumer();
    consumer();
    consumer();
    consumer();

    return 0;
}