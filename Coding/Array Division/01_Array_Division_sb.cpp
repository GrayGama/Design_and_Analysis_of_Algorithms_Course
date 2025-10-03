#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>

using namespace std;

bool is_valid_partition(const vector<int>& array, int k, long long max_sum) {
    long long curr_sum = 0;
    int subarray_count = 1;

    for (int elem : array) {
        if (curr_sum + elem > max_sum) {
            subarray_count++;
            curr_sum = elem;
            if (subarray_count > k) { return false; }
        } else {
            curr_sum += elem;
        }
    }
    return true;
}

long long max_subarray_sum(const vector<int>& array, int k) {
    long long left = *max_element(array.begin(), array.end());
    long long right = 0;
    for (int elem : array) { right += elem; }
    long long result = right;   // I used long long since the sum can exceed the maximum value of int, and long, for some testcases.  

    while (left <= right) {
        long long mid = left + (right - left) / 2;

        if (is_valid_partition(array, k, mid)) {
            result = mid;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    return result;
}

int main(){
    // --> For one or multiple files execution
    // Make sure to make the outputs1/ directory.
    for (int file_num = 1; file_num <= 10; file_num++) {    
        string filename = "tests1/" + to_string(file_num) + ".in";
        ifstream infile(filename);
        if (!infile) {return 1;}

        int n, k;
        infile >> n >> k;
        vector<int> array(n);
        for (int i = 0; i < n; ++i) {
            infile >> array[i];
        }
        infile.close();

        string out_filename = "outputs1/" + to_string(file_num) + ".out"; 
        ofstream outfile (out_filename);
        outfile << max_subarray_sum(array, k) << endl;
        outfile.close();

    }

    return 0;
}