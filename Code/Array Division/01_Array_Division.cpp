#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

bool is_valid_partition(const vector<int>& array, int k, int max_sum) {
    int current_sum = 0;
    int subarray_count = 1;

    for (int num : array) {
        if (current_sum + num > max_sum) {
            subarray_count++;
            current_sum = num;
            cout << "w-current sum: " << current_sum << endl;
            if (subarray_count > k) {
                return false;
            }
        } else {
            current_sum += num;
            cout << "e-current sum: " << current_sum << endl;
        }
    }
    cout << "sub array count: " << subarray_count << " "; 
    cout << "current sum: " << current_sum << endl;
    return true;
}

int max_subarray_sum(const vector<int>& array, int k) {
    int left = *max_element(array.begin(), array.end());
    int right = accumulate(array.begin(), array.end(), 0);
    int result = right;
    cout << "result: " << result << endl; 

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (is_valid_partition(array, k, mid)) {
            result = mid;
            cout << "w-result: " << result << endl;
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    return result;
}

int main(){
    int n, k;
    cin >> n >> k;
    vector<int> array(n);

    for (int i = 0; i < n; ++i) {
        cin >> array[i];
    }

    cout << max_subarray_sum(array, k) << endl;

    return 0;
}