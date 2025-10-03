#include <iostream>
#include <vector>
#include <queue>
#include <string>

using namespace std;

// Helper function to check if placing a queen is valid
bool isSafe(const vector<int>& queens, int row, int col) {
    for (int i = 0; i < row; ++i) {
        int placedCol = queens[i];
        if (placedCol == col || abs(placedCol - col) == abs(i - row)) {
            return false; // Same column or diagonal
        }
    }
    return true;
}

// BFS function to solve the n-Queens problem
int solveNQueens(int n) {
    queue<vector<int>> states; // Queue to hold states (rows completed)
    states.push(vector<int>()); // Start with an empty board
    int solutionCount = 0;

    while (!states.empty()) {
        vector<int> current = states.front();
        states.pop();

        int row = current.size(); // Current row to place the queen
        if (row == n) {
            ++solutionCount; // Found a valid solution
        } else {
            for (int col = 0; col < n; ++col) {
                if (isSafe(current, row, col)) {
                    vector<int> nextState = current;
                    nextState.push_back(col); // Place queen in this column
                    states.push(nextState);
                }
            }
        }
    }
    return solutionCount;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <N>" << endl;
        return 1;
    }

    int n = stoi(argv[1]);
    if (n < 8) {
        cerr << "N must be >= 8" << endl;
        return 1;
    }

    int solutionCount = solveNQueens(n);
    cout << solutionCount << endl;

    return 0;
}