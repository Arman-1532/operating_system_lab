#include <iostream>
#include <condition_variable>
#include <mutex>
using namespace std;

int readers = 0;
bool writerActive = false;
condition_variable cv;
mutex m;

void ReaderEnter(int id) {
    unique_lock<mutex> lock(m);
    cv.wait(lock, [] { return !writerActive; });
    readers++;
    cout << "Reader " << id << " enters. Readers: " << readers << endl;
}

void ReaderExit(int id) {
    unique_lock<mutex> lock(m);
    readers--;
    cout << "Reader " << id << " exits. Readers: " << readers << endl;
    if (readers == 0) cv.notify_one();
}

void WriterEnter(int id) {
    unique_lock<mutex> lock(m);
    cv.wait(lock, [] { return !writerActive && readers == 0; });
    writerActive = true;
    cout << "Writer " << id << " enters." << endl;
}

void WriterExit(int id) {
    unique_lock<mutex> lock(m);
    writerActive = false;
    cout << "Writer " << id << " exits." << endl;
    cv.notify_all();
}

int main() {
    ReaderEnter(1);
    ReaderExit(1);
    WriterEnter(1);
    WriterExit(1);
    ReaderEnter(2);
    ReaderExit(2);
    WriterEnter(2);
    WriterExit(2);
    ReaderEnter(3);
    ReaderExit(3);
}
