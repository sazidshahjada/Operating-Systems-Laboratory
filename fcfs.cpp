#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Structure to represent a process
struct Process {
    string name;
    int arrival;
    int burst;
};

// Function to implement FCFS scheduling
void fcfs(const vector<Process>& processes) {
    int current_time = 0;
    double total_wt = 0.0;
    double total_tat = 0.0;
    int n = processes.size();

    cout << "FCFS Scheduling Results:" << endl;
    for (const auto& p : processes) {
        if (current_time < p.arrival) {
            current_time = p.arrival;
        }
        int wt = current_time - p.arrival;
        int tat = wt + p.burst;
        total_wt += wt;
        total_tat += tat;
        cout << "Process " << p.name << ": Arrival=" << p.arrival << ", Burst=" << p.burst
             << ", Waiting=" << wt << ", Turnaround=" << tat << endl;
        current_time += p.burst;
    }
    cout << "Average Waiting Time: " << total_wt / n << endl;
    cout << "Average Turnaround Time: " << total_tat / n << endl;
}

int main() {
    vector<Process> processes = {
        {"P1", 0, 10},
        {"P2", 1, 5},
        {"P3", 2, 8},
        {"P4", 3, 6},
        {"P5", 4, 2}
    };

    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrival < b.arrival;
    });

    fcfs(processes);

    return 0;
}