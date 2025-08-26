#include <iostream>
#include <vector>
using namespace std;

class Bankers {
    int n, m;  // n = processes, m = resources
    vector<vector<int>> allocation, maximum, need;
    vector<int> available;

public:
    Bankers(int n, int m,
            vector<vector<int>> allocation,
            vector<vector<int>> maximum,
            vector<int> available)
        : n(n), m(m), allocation(allocation), maximum(maximum), available(available) 
    {
        need.resize(n, vector<int>(m));
        // Calculate need matrix
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                need[i][j] = maximum[i][j] - allocation[i][j];
    }

    // Check if system is in safe state
    bool isSafe() {
        vector<int> work = available;
        vector<bool> finish(n, false);
        vector<int> safeSeq;

        while (safeSeq.size() < n) {
            bool found = false;
            for (int i = 0; i < n; i++) {
                if (!finish[i]) {
                    bool canAllocate = true;
                    for (int j = 0; j < m; j++) {
                        if (need[i][j] > work[j]) {
                            canAllocate = false;
                            break;
                        }
                    }
                    if (canAllocate) {
                        // Pretend to allocate
                        for (int j = 0; j < m; j++)
                            work[j] += allocation[i][j];
                        safeSeq.push_back(i);
                        finish[i] = true;
                        found = true;
                    }
                }
            }
            if (!found) {
                cout << "System is NOT in a safe state (Deadlock possible)." << endl;
                return false;
            }
        }

        cout << "System is in a SAFE state.\nSafe Sequence: ";
        for (int i = 0; i < n; i++)
            cout << "P" << safeSeq[i] << (i == n - 1 ? "" : " -> ");
        cout << endl;
        return true;
    }

    // Request resources for a process
    void requestResources(int p, vector<int> request) {
        cout << "\nProcess P" << p << " requesting: ";
        for (int r : request) cout << r << " ";
        cout << endl;

        // Check if request <= need
        for (int j = 0; j < m; j++) {
            if (request[j] > need[p][j]) {
                cout << "Error: Process has exceeded its maximum claim!" << endl;
                return;
            }
        }

        // Check if request <= available
        for (int j = 0; j < m; j++) {
            if (request[j] > available[j]) {
                cout << "Process must wait: Resources not available." << endl;
                return;
            }
        }

        // Pretend to allocate
        for (int j = 0; j < m; j++) {
            available[j] -= request[j];
            allocation[p][j] += request[j];
            need[p][j] -= request[j];
        }

        if (isSafe()) {
            cout << "Request can be granted safely." << endl;
        } else {
            // Rollback
            for (int j = 0; j < m; j++) {
                available[j] += request[j];
                allocation[p][j] -= request[j];
                need[p][j] += request[j];
            }
            cout << "Request CANNOT be granted (would lead to unsafe state)." << endl;
        }
    }
};

int main() {
    // Example input
    int n = 5, m = 3; // 5 processes, 3 resources
    vector<vector<int>> allocation = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };

    vector<vector<int>> maximum = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };

    vector<int> available = {3, 3, 2};

    Bankers bankers(n, m, allocation, maximum, available);
    bankers.isSafe();

    // Example request
    vector<int> request = {1, 0, 2};
    bankers.requestResources(1, request);

    return 0;
}
