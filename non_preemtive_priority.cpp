#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

// Structure to represent a process
struct Process {
    string name;
    int arrival;
    int burst;
    int priority;
};

// Function to implement Non-Preemptive Priority scheduling
void non_preemptive_priority(const vector<Process>& processes) {
    int current_time = 0;
    double total_wt = 0.0;
    double total_tat = 0.0;
    int n = processes.size();
    vector<bool> completed(n, false);

    cout << "Non-Preemptive Priority Scheduling Results:" << endl;
    int done = 0;
    while (done < n) {
        int selected = -1;
        int min_priority = INT_MAX;
        for (int i = 0; i < n; ++i) {
            if (processes[i].arrival <= current_time && !completed[i] && processes[i].priority < min_priority) {
                min_priority = processes[i].priority;
                selected = i;
            }
        }
        if (selected == -1) {
            current_time++;
            continue;
        }
        const Process& p = processes[selected];
        int wt = current_time - p.arrival;
        int tat = wt + p.burst;
        total_wt += wt;
        total_tat += tat;
        cout << "Process " << p.name << ": Arrival=" << p.arrival << ", Burst=" << p.burst
             << ", Priority=" << p.priority << ", Waiting=" << wt << ", Turnaround=" << tat << endl;
        current_time += p.burst;
        completed[selected] = true;
        done++;
    }
    cout << "Average Waiting Time: " << total_wt / n << endl;
    cout << "Average Turnaround Time: " << total_tat / n << endl;
}

int main() {
    vector<Process> processes = {
        {"P1", 0, 10, 2},
        {"P2", 1, 5, 5},
        {"P3", 2, 8, 1},
        {"P4", 3, 6, 4},
        {"P5", 4, 2, 3}
    };

    non_preemptive_priority(processes);

    return 0;
}