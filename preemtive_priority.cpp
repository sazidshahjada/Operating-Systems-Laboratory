#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <iomanip>

using namespace std;

// Structure to represent a process
struct Process {
    string name;
    int arrival;
    int burst;
    int priority;
    int remaining;
    int completion = 0;
};

// Function to implement Preemptive Priority Scheduling
void preemptive_priority(vector<Process> processes) {
    int n = processes.size();
    int current_time = 0, completed = 0;

    for (int i = 0; i < n; i++) processes[i].remaining = processes[i].burst;

    vector<int> gantt_time;      // timeline
    vector<string> gantt_proc;   // executed process names

    while (completed < n) {
        int selected = -1, min_priority = INT_MAX;

        // Select highest-priority process available
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival <= current_time && processes[i].remaining > 0) {
                if (processes[i].priority < min_priority) {
                    min_priority = processes[i].priority;
                    selected = i;
                } else if (processes[i].priority == min_priority) {
                    if (processes[i].arrival < processes[selected].arrival) {
                        selected = i;
                    }
                }
            }
        }

        if (selected == -1) {
            // CPU idle
            gantt_time.push_back(current_time);
            gantt_proc.push_back("Idle");
            current_time++;
            continue;
        }

        // Execute 1 unit of the selected process
        gantt_time.push_back(current_time);
        gantt_proc.push_back(processes[selected].name);

        processes[selected].remaining--;
        current_time++;

        if (processes[selected].remaining == 0) {
            processes[selected].completion = current_time;
            completed++;
        }
    }

    gantt_time.push_back(current_time); // mark final timestamp

    // ---- Print results in your style ----
    double total_wt = 0, total_tat = 0;
    cout << "Preemptive Priority Scheduling Results:" << endl;
    for (int i = 0; i < n; i++) {
        int tat = processes[i].completion - processes[i].arrival;
        int wt = tat - processes[i].burst;
        total_wt += wt;
        total_tat += tat;

        cout << "Process " << processes[i].name
             << ": Arrival=" << processes[i].arrival
             << ", Burst=" << processes[i].burst
             << ", Priority=" << processes[i].priority
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
        {"P1", 0, 10, 2},
        {"P2", 1, 5, 5},
        {"P3", 2, 8, 1},
        {"P4", 3, 6, 4},
        {"P5", 4, 2, 3}
    };

    // Sort by arrival time
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrival < b.arrival;
    });

    preemptive_priority(processes);
    return 0;
}
