#include <iostream>
   
using namespace std;

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0, out = 0, counter = 0;
int produces = 1;
int consumed = 0;
int total_produced = 0;
int total_consumed = 0;
const int MAX_ITEMS = 10;

void producer() {
    if (counter < BUFFER_SIZE) {
        buffer[in] = produces;
        cout << "Produced: " << produces << " at " << in << endl;
        in = (in + 1) % BUFFER_SIZE;
        counter++;
        produces++;
        total_produced++;
    }
    else {
        cout << "Buffer is full" << endl;
    }
}

void consumer() {
    if (counter > 0) {
        consumed = buffer[out];
        cout << "Consumed: " << consumed << " from " << out << endl;
        out = (out + 1) % BUFFER_SIZE;
        counter--;
        total_consumed++;
    }
    else {
        cout << "Buffer is empty" << endl;
    }
}

int main() {
    

    producer();
    consumer();
    producer();
    producer();
    consumer();
    producer();
    producer();
    consumer();
    producer();
    producer();
    consumer();
    producer();
    producer();
    consumer();
    producer();
    producer();
    consumer();
    producer();
    producer();
    consumer();
    producer();
    producer();
    consumer();
    producer();
    producer();
    consumer();
    producer();
    producer();
    consumer();
    producer();
    

    return 0;
}
