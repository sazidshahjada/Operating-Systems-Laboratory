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
        // Need = Max - Allocation
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                need[i][j] = maximum[i][j] - allocation[i][j];
    }

    // Safety Algorithm
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
                cout << "System is NOT in a safe state. Deadlock possible.\n";
                return false;
            }
        }

        cout << "System is in a SAFE state.\nSafe Sequence: ";
        for (int i = 0; i < n; i++)
            cout << "P" << safeSeq[i] << (i == n - 1 ? "" : " -> ");
        cout << "\n";
        return true;
    }

    // Resource request algorithm (Deadlock Avoidance)
    void requestResources(int p, vector<int> request) {
        cout << "\nProcess P" << p << " requesting: ";
        for (int r : request) cout << r << " ";
        cout << "\n";

        // Step 1: Check if request <= need
        for (int j = 0; j < m; j++) {
            if (request[j] > need[p][j]) {
                cout << "Error: Process exceeded its maximum claim!\n";
                return;
            }
        }

        // Step 2: Check if request <= available
        for (int j = 0; j < m; j++) {
            if (request[j] > available[j]) {
                cout << "Process must WAIT: Resources not available.\n";
                return;
            }
        }

        // Step 3: Pretend to allocate
        for (int j = 0; j < m; j++) {
            available[j] -= request[j];
            allocation[p][j] += request[j];
            need[p][j] -= request[j];
        }

        // Step 4: Safety check
        if (isSafe()) {
            cout << "Request can be GRANTED safely.\n";
        } else {
            // Rollback
            for (int j = 0; j < m; j++) {
                available[j] += request[j];
                allocation[p][j] -= request[j];
                need[p][j] += request[j];
            }
            cout << "Request DENIED: Would lead to unsafe state.\n";
        }
    }
};

int main() {
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

    // Initial safety check
    bankers.isSafe();

    // Example request
    vector<int> req1 = {1, 0, 2};
    bankers.requestResources(1, req1);

    vector<int> req2 = {3, 3, 0};
    bankers.requestResources(4, req2);

    return 0;
}
