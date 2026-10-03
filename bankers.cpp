#include <iostream>
#include <vector>
#include <queue>
using namespace std;

struct Process {
    int id;
    vector<int> alloc;
    vector<int> maxReq;
    Process(int i, const vector<int>& a, const vector<int>& m) : id(i), alloc(a), maxReq(m) {}
};

int n = 3; //no of process
int r = 3; // no of resource


vector<Process> processes = {  //id Allocation and MaxNeed
    Process(0, {0, 1, 0}, {7, 5, 3}),
    Process(1, {2, 0, 0}, {3, 2, 2}),
    Process(2, {3, 0, 2}, {9, 0, 2})
};

vector<int> totalSystemResources = {10, 5, 7}; 

vector<int> available(r, 0);
vector<Process> finished;

void calculateAvailable() {
    vector<int> allocatedSum(r, 0);
    for (auto &p : processes)
        for (int i = 0; i < r; i++)
            allocatedSum[i] += p.alloc[i];
    for (int i = 0; i < r; i++)
        available[i] = totalSystemResources[i] - allocatedSum[i];
}

void runBanker() {
    queue<Process> q;
    for (auto &p : processes) q.push(p);

    int stuck = 0;
    while (!q.empty()) {
        Process p = q.front(); q.pop();
        bool canRun = true;

        for (int i = 0; i < r; i++) {
            int need = p.maxReq[i] - p.alloc[i];
            if (need > available[i]) {
                canRun = false;
                break;
            }
        }

        if (canRun) {
            for (int i = 0; i < r; i++) available[i] += p.alloc[i];
            finished.push_back(p);
            stuck = 0;
        } else {
            q.push(p);
            stuck++;
        }

        if (stuck >= (int)q.size() && !q.empty()) {
            cout << "Deadlock detected!\n";
            break;
        }
    }
}

void printResult() {
    for (auto &p : finished) {
        cout  << "P"<< p.id << " ";
    }
    cout << "\n";
}

int main() {
    calculateAvailable();
    runBanker();
    printResult();
    return 0;
}
