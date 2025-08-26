#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <climits>

using namespace std;

struct Process {
    string name;
    int arrival;
    int burst;
    int remaining;
    int completion = 0;
};

// Round Robin Scheduling
void round_robin(vector<Process> processes, int quantum) {
    int n = processes.size();
    int current_time = 0, completed = 0;

    for (int i = 0; i < n; i++) processes[i].remaining = processes[i].burst;

    queue<int> ready;
    vector<int> gantt_time;
    vector<string> gantt_proc;

    vector<bool> inQueue(n, false);

    // push first arrivals
    for (int i = 0; i < n; i++) {
        if (processes[i].arrival == 0) {
            ready.push(i);
            inQueue[i] = true;
        }
    }

    while (completed < n) {
        if (ready.empty()) {
            // idle CPU, jump to next arrival
            gantt_time.push_back(current_time);
            gantt_proc.push_back("Idle");

            int next_arrival = INT_MAX;
            for (int i = 0; i < n; i++) {
                if (processes[i].remaining > 0) {
                    next_arrival = min(next_arrival, processes[i].arrival);
                }
            }
            current_time = max(current_time + 1, next_arrival);

            // enqueue processes that have arrived
            for (int i = 0; i < n; i++) {
                if (!inQueue[i] && processes[i].arrival <= current_time && processes[i].remaining > 0) {
                    ready.push(i);
                    inQueue[i] = true;
                }
            }
            continue;
        }

        int idx = ready.front();
        ready.pop();

        int exec_time = min(quantum, processes[idx].remaining);
        for (int t = 0; t < exec_time; t++) {
            gantt_time.push_back(current_time);
            gantt_proc.push_back(processes[idx].name);
            current_time++;
        }

        processes[idx].remaining -= exec_time;

        // enqueue new arrivals that came during this slice
        for (int i = 0; i < n; i++) {
            if (!inQueue[i] && processes[i].arrival <= current_time && processes[i].remaining > 0) {
                ready.push(i);
                inQueue[i] = true;
            }
        }

        if (processes[idx].remaining > 0) {
            ready.push(idx); // requeue
        } else {
            processes[idx].completion = current_time;
            completed++;
        }
    }

    gantt_time.push_back(current_time);

    // ---- Print results in your style ----
    double total_wt = 0, total_tat = 0;
    cout << "Round Robin Scheduling Results (Quantum=" << quantum << "):" << endl;
    for (int i = 0; i < n; i++) {
        int tat = processes[i].completion - processes[i].arrival;
        int wt = tat - processes[i].burst;
        total_wt += wt;
        total_tat += tat;

        cout << "Process " << processes[i].name
             << ": Arrival=" << processes[i].arrival
             << ", Burst=" << processes[i].burst
             << ", Waiting=" << wt
             << ", Turnaround=" << tat << endl;
    }

    cout << "Average Waiting Time: " << total_wt / n << endl;
    cout << "Average Turnaround Time: " << total_tat / n << endl;

    // ---- Print Gantt Chart ----
    cout << "\nGantt Chart:" << endl;
    for (size_t i = 0; i < gantt_proc.size(); i++) {
        cout << setw(3) << gantt_time[i] << " [" << gantt_proc[i] << "]";
    }
    cout << " " << gantt_time.back() << endl;
}

int main() {
    vector<Process> processes = {
        {"P1", 0, 10},
        {"P2", 1, 5},
        {"P3", 2, 8},
        {"P4", 3, 6},
        {"P5", 4, 2}
    };

    // Sort by arrival time
    sort(processes.begin(), processes.end(), [](const Process &a, const Process &b) {
        return a.arrival < b.arrival;
    });

    round_robin(processes, 2); // Quantum = 2
    return 0;
}
