// #include <bits/stdc++.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

/*
Each Layer has:
- arr:    sorted array of size 2^k (if nonempty)
- deleted: same size, marking which slots are 'logically' removed

If arr is empty, the layer is considered empty (no elements).
Otherwise it must hold exactly 2^k elements.
*/
struct Layer {
    vector<int> arr;        // sorted list of elements
    vector<bool> deleted;   // deleted[i] == true if arr[i] is logically removed

    bool isEmpty() const { return arr.empty(); }

    int liveCount() const {
        if (isEmpty()) return 0;
        int count = 0;
        for (size_t i = 0; i < arr.size(); i++) {
            if (!deleted[i]) count++;
        }
        return count;
    }
};

class DS_Layers {
private:
    static const int MAX_LAYERS = 16; // up to 2^16 elements
    vector<Layer> layers; // A vector of layers. Index k means capacity = 2^k, if not empty.

public:
    DS_Layers() { layers.resize(MAX_LAYERS); }

    void build(const vector<int>& initVals) {
        for (int val : initVals) {
            insert(val);
        }
    }

    pair<int, int> search(int x) const {
        for (int k = 0; k < MAX_LAYERS; k++) {
            if (layers[k].isEmpty()) continue;

            const auto &arr = layers[k].arr;
            const auto &del = layers[k].deleted;
            int left = 0;
            int right = (int)arr.size() - 1;
            while (left <= right) {
                int mid = (left + right) / 2;
                if (arr[mid] == x) {
                    if (!del[mid]) {
                        return make_pair(k, mid);   // Found alive at layer k, index mid
                    } else {
                        break;  // Found but deleted => treat as not found
                    }
                }
                else if (arr[mid] < x) {
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
        }
        return make_pair(-1, -1);  // not found
    }

    bool insert(int x) {
        pair<int, int> searchResult = search(x);

        if (searchResult.first != 1) { return false; }    // Check for duplicates, x is found, and no insertion

        vector<int> carry{x}; 

        for (int k = 0; k < MAX_LAYERS; k++) {
            if (layers[k].isEmpty()) {   // If the layer is empty, just store carry here and it's done
                layers[k].arr = carry;
                layers[k].deleted.assign(carry.size(), false);
                return true;
            } else {
                // Do the merge step
                vector<int> mergedArr = mergeSortedArr(layers[k].arr, layers[k].deleted, carry);

                layers[k].arr.clear();
                layers[k].deleted.clear();

                carry = move(mergedArr); // carry the next layer of our binomial array
            }
        }
        return true;
    }

    // Delete x if present. Return true if found and deleted, false if not found.
    bool remove(int x) {
        
        pair<int, int> searchResult = search(x);
        int idx = searchResult.first;
        int pos = searchResult.second;

        if (idx == -1) return false;    // Didn't find x

         // 2) Mark deleted
        layers[idx].deleted[pos] = true;

        // 
        int live = layers[idx].liveCount();
        int capacity = (int)layers[idx].arr.size();
        if (live * 2 < capacity) {
            vector<int> liveElements = gatherLive(  // Gather the alive elements
                layers[idx].arr,
                layers[idx].deleted
            );
            layers[idx].arr.clear();
            layers[idx].deleted.clear();
           
            mergeUp(idx + 1, liveElements);  // Merge these live elements into idx+1
        }
        return true;
    }

private:
    static vector<int> mergeSortedArr(const vector<int>& A, const vector<bool>& deletedA, const vector<int>& B) {
        vector<int> A_live;
        for (size_t i = 0; i < A.size(); i++) {
            if (!deletedA[i]) { A_live.push_back(A[i]); }
        }
        vector<int> result;     // Merge A_live and B (both sorted)
        result.reserve(A_live.size() + B.size());

        merge(A_live.begin(), A_live.end(),    // From <algorithm> std::merge
                B.begin(), B.end(),
                back_inserter(result));

        return result;
    }

    // Merge "carry" array of sorted elements into layer L (by index).
    // If L is empty, place carry there. If L is full, merge and carry up again.
    void mergeUp(int L, vector<int> carry) {
        for (int k = L; k < MAX_LAYERS; k++) {
            if (layers[k].isEmpty()) {
                layers[k].arr = carry;
                layers[k].deleted.assign(carry.size(), false);
                return;
            } else {
                vector<int> merged = mergeSortedArr(layers[k].arr, layers[k].deleted, carry);
                
                layers[k].arr.clear();  // empty out layer k
                layers[k].deleted.clear();
                carry = move(merged);
            }
        }
    }
    
    static vector<int> gatherLive(const vector<int>& arr, const vector<bool>& del) {
        
        vector<int> result;
        for (size_t i = 0; i < arr.size(); i++) {
            if (!del[i]) result.push_back(arr[i]);
        }
        return result;
    }
};
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> data(n);
    for (int i = 0; i < n; i++) {
        cin >> data[i];
    }

    DS_Layers ds;
    ds.build(data);

    int m;
    cin >> m;
    vector<pair<char, int>> cmds(m);
    for (int i = 0; i < m; ++i) {
        cin >> cmds[i].first >> cmds[i].second;
    }

    for (const auto& cmd : cmds) {
        char op = cmd.first;
        int val = cmd.second;

        if (op == 's') {
            // search
            pair<int, int> searchResult = ds.search(val);; 
            int ly = searchResult.first;
            if (ly != -1) {
                // Found
                cout << "Found\n";
            } else {
                cout << "Not Found\n";
            }
        } else if (op == 'd') {
            // delete
            if (ds.remove(val)) {
                cout << "Delete Success\n";
            } else {
                cout << "Delete Failed\n";
            }
        } else if (op == 'i') {
            // insert
            if (ds.insert(val)) {
                cout << "Insert Success\n";
            } else {
                cout << "Insert Failed\n";
            }
        }
    }

    return 0;
}
