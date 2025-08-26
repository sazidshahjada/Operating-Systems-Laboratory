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
    int remaining;
    int completion = 0;
};

// Shortest Remaining Time First Scheduling
void srtf(vector<Process> processes) {
    int n = processes.size();
    int current_time = 0, completed = 0;
    vector<bool> done(n, false);

    vector<int> gantt_time;
    vector<string> gantt_proc;

    for (auto &p : processes) p.remaining = p.burst;

    while (completed < n) {
        int idx = -1;
        int min_remaining = INT_MAX;

        // Find available process with shortest remaining time
        for (int i = 0; i < n; i++) {
            if (!done[i] && processes[i].arrival <= current_time) {
                if (processes[i].remaining < min_remaining) {
                    min_remaining = processes[i].remaining;
                    idx = i;
                }
                else if (processes[i].remaining == min_remaining) {
                    // Tie-breaker: earlier arrival
                    if (processes[i].arrival < processes[idx].arrival) {
                        idx = i;
                    }
                }
            }
        }

        if (idx == -1) {
            // CPU idle
            if (gantt_proc.empty() || gantt_proc.back() != "Idle") {
                gantt_time.push_back(current_time);
                gantt_proc.push_back("Idle");
            }
            current_time++;
            continue;
        }

        // Context switch (if new process is chosen)
        if (gantt_proc.empty() || gantt_proc.back() != processes[idx].name) {
            gantt_time.push_back(current_time);
            gantt_proc.push_back(processes[idx].name);
        }

        // Execute 1 unit of time
        processes[idx].remaining--;
        current_time++;

        if (processes[idx].remaining == 0) {
            processes[idx].completion = current_time;
            done[idx] = true;
            completed++;
        }
    }

    gantt_time.push_back(current_time);

    // ---- Print Results ----
    double total_wt = 0, total_tat = 0;
    cout << "SRTF (Preemptive SJF) Scheduling Results:" << endl;
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
        {"P1", 0, 8},
        {"P2", 1, 4},
        {"P3", 2, 9},
        {"P4", 3, 5}
    };

    // Sort by arrival time
    sort(processes.begin(), processes.end(), [](const Process &a, const Process &b) {
        return a.arrival < b.arrival;
    });

    srtf(processes);
    return 0;
}
