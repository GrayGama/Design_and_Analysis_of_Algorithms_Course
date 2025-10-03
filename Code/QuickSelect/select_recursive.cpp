#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <string>

using namespace std;

// Partition function
int partition(vector<int>& arr, int left, int right, int pivotIdx) {
    int pivotValue = arr[pivotIdx];
    swap(arr[pivotIdx], arr[right]); // Move pivot to end
    int storeIdx = left;
    for (int i = left; i < right; ++i) {
        if (arr[i] < pivotValue) {
            swap(arr[i], arr[storeIdx]);
            ++storeIdx;
        }
    }
    swap(arr[storeIdx], arr[right]); // Move pivot to its final place
    
    return storeIdx;
}

// Selects pivot using the median of medians method
int selectPivot(vector<int>& arr, int left, int right, int g) {
    if (right - left + 1 <= g) {
        sort(arr.begin() + left, arr.begin() + right + 1); // Sort and pick middle
        return (left + right) / 2;
    }

    // Split into groups of size g, find medians
    vector<int> medians;
    for (int i = left; i <= right; i += g) {
        int subRight = min(i + g - 1, right);
        int medianIdx = (i + subRight) / 2;
        nth_element(arr.begin() + i, arr.begin() + medianIdx, arr.begin() + subRight + 1);
        medians.push_back(arr[medianIdx]);
    }

    // Recursively find the median of the medians
    return selectPivot(medians, 0, medians.size() - 1, g);
}

// Main selection algorithm (Quickselect with optional randomization)
int select(vector<int>& arr, int left, int right, int k, int g, bool randomized) {
    random_device rd;
    mt19937 gen(rd());

     // Base case: if there's only one element, return it
    if (left == right) {
        return arr[left];
    }
    int pivotIdx = randomized ? uniform_int_distribution<>(left, right)(gen) : selectPivot(arr, left, right, g);
    pivotIdx = partition(arr, left, right, pivotIdx);

    if (pivotIdx == k) {
        return arr[pivotIdx];
    } else if (pivotIdx > k) {
        return select(arr, left, pivotIdx - 1, k, g, randomized);
    } else {
        return select(arr, pivotIdx + 1, right, k, g, randomized);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int N, K, G;
    cin >> N >> K >> G;

    // Generation of a random array
    vector<int> arr(N);
    for(int i = 0; i < N; i++){
        cin >> arr[i];
    }

    vector<int> groupSizes = {3, 5, 7, 9, -1};  // -1 for randomized select
    
    // for (int g : groupSizes) {
    int g = G; 
    long long totalTime = 0;
    const int experiments = 100;

    for (int i = 0; i < experiments; ++i) {
        vector<int> copyArr = arr;  // Create a fresh copy of arr for each experiment
        auto start = chrono::high_resolution_clock::now();
        int result = select(copyArr, 0, N - 1, K - 1, g, g == -1);
        auto end = chrono::high_resolution_clock::now();
        totalTime += chrono::duration_cast<chrono::microseconds>(end - start).count();

        // Move this print inside to ensure correct Kth smallest is printed each time
        if (i == experiments - 1) {  // Only print result for last iteration (optional)
            cout 
                // << "Group Size: " << (g == -1 ? "Randomized" : to_string(g))
                // << "\nKth Smallest: " 
                << result << endl;
                // << "\nAverage Time: " << totalTime / experiments << " microseconds\n\n";
        }
    }
    // }

    return 0;
}