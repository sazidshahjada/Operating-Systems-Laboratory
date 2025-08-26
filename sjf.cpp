#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <climits>
using namespace std;

struct Process {
    string name;
    int arrival;
    int burst;
    int completion = 0;
};

// SJF Non-Preemptive Scheduling
void sjf_non_preemptive(vector<Process> processes) {
    int n = processes.size();
    int current_time = 0, completed = 0;

    vector<int> gantt_time;
    vector<string> gantt_proc;
    vector<bool> done(n, false);

    while (completed < n) {
        int idx = -1;
        int min_burst = INT_MAX;

        // Select available process with shortest burst
        for (int i = 0; i < n; i++) {
            if (!done[i] && processes[i].arrival <= current_time) {
                if (processes[i].burst < min_burst) {
                    min_burst = processes[i].burst;
                    idx = i;
                }
                else if (processes[i].burst == min_burst) {
                    // tie-breaker: earlier arrival
                    if (processes[i].arrival < processes[idx].arrival) {
                        idx = i;
                    }
                }
            }
        }

        if (idx == -1) {
            // CPU idle
            gantt_time.push_back(current_time);
            gantt_proc.push_back("Idle");
            current_time++;
            continue;
        }

        // Run process fully (non-preemptive)
        gantt_time.push_back(current_time);
        gantt_proc.push_back(processes[idx].name);

        current_time += processes[idx].burst;
        processes[idx].completion = current_time;
        done[idx] = true;
        completed++;
    }

    gantt_time.push_back(current_time);

    // ---- Print results ----
    double total_wt = 0, total_tat = 0;
    cout << "SJF (Non-Preemptive) Scheduling Results:" << endl;
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

    // Sort by arrival
    sort(processes.begin(), processes.end(), [](const Process &a, const Process &b) {
        return a.arrival < b.arrival;
    });

    sjf_non_preemptive(processes);
    return 0;
}
