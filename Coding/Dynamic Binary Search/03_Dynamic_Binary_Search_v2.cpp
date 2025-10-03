#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

struct Layer {
    vector<int> arr;        // sorted list of elements
    vector<bool> deleted;   // deleted[i] == true if arr[i] is logically removed

    bool isEmpty() const {
        return arr.empty();
    }

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
    static const int MAX_LAYERS = 16; 
    vector<Layer> layers;   // A vector of layers. Index k => capacity = 2^k (if not empty)

public:
    DS_Layers() { layers.resize(MAX_LAYERS); }

    void build(const vector<int>& initVals) {
        for (int val : initVals) {
            insert(val);
        }
    }

    pair<int, int> search(int x) const {
        // Binary Search
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
                        return make_pair(k, mid);   // "Found" alive at layer k, index mid
                    } else {
                        break;  // Found but deleted, treat as not found
                    }
                }
                else if (arr[mid] < x) {
                    left = mid + 1;
                } else {
                    right = mid - 1;
                }
            }
        }
        return make_pair(-1, -1);  // "Not Found"
    }

    bool insert(int x) {
        // Binomial Array 
        pair<int, int> searchResult = search(x); 
        
        if (searchResult.first != -1) { return false; }   // Check for duplicates, x is found, and no insertion

        vector<int> carry{ x }; 
        
        for (int k = 0; k < MAX_LAYERS; k++) {
            if (layers[k].isEmpty()) {  // If the layer is empty, just store carry here and it's done
                layers[k].arr = carry;
                layers[k].deleted.assign(carry.size(), false);
                return true;
            } else {    // Merge carry with the live elements in layers[k]
                vector<int> mergedArr = mergeSortedArr(layers[k].arr, layers[k].deleted, carry); 
                // Clear out the layer
                layers[k].arr.clear();
                layers[k].deleted.clear();

                carry = move(mergedArr);   // Carry the merged array upward
            }
        }
        // In case of overflow store in the last layer
        layers[MAX_LAYERS - 1].arr = carry;
        layers[MAX_LAYERS - 1].deleted.assign(carry.size(), false);
        return true;
    }

    bool remove(int x) {
        // (1) Locate x
        pair<int, int> searchResult = search(x); 
        int layerIdx = searchResult.first;
        int pos = searchResult.second;
        if (layerIdx == -1) { return false; }

        // (2) Mark as deleted
        layers[layerIdx].deleted[pos] = true;

        // (3) Check if more than half are deleted in that layer
        int live = layers[layerIdx].liveCount();
        int capacity = (int)layers[layerIdx].arr.size();
        if (live * 2 < capacity) {
            vector<int> liveElements = gatherLive(layers[layerIdx].arr, layers[layerIdx].deleted);    // gather alive, clear layer, merge up
            
            layers[layerIdx].arr.clear();
            layers[layerIdx].deleted.clear();

            mergeUp(layerIdx + 1, liveElements);
        }
        return true;
    }

private:
    static vector<int> mergeSortedArr(const vector<int>& A, const vector<bool>& deletedA, const vector<int>& B) {
        vector<int> A_live;
        A_live.reserve(A.size());
        for (size_t i = 0; i < A.size(); i++) {
            if (!deletedA[i]) {
                A_live.push_back(A[i]);
            }
        }
        
        vector<int> result;
        result.reserve(A_live.size() + B.size());
        merge(A_live.begin(), A_live.end(),     // Now merge A_live and B (both are sorted)
              B.begin(), B.end(),
              back_inserter(result));

        return result;
    }

    
    void mergeUp(int L, vector<int> carry) {
        // Merge 'carry' into layer L. If layer L is full, merge again and carry upward.
        for (int k = L; k < MAX_LAYERS; k++) {
            if (layers[k].isEmpty()) {
                layers[k].arr = carry;
                layers[k].deleted.assign(carry.size(), false);
                return;
            } else {
                vector<int> merged = mergeSortedArr(layers[k].arr, layers[k].deleted, carry);
                layers[k].arr.clear();
                layers[k].deleted.clear();
                carry = move(merged);
            }
        }
        // Overflow management
        layers[MAX_LAYERS - 1].arr = carry;
        layers[MAX_LAYERS - 1].deleted.assign(carry.size(), false);
    }

    static vector<int> gatherLive(const vector<int>& arr, const vector<bool>& del) {
        // Gather the live elements from arr using a parallel deleted vector
        vector<int> result;
        result.reserve(arr.size());
        for (size_t i = 0; i < arr.size(); i++) {
            if (!del[i]) {
                result.push_back(arr[i]);
            }
        }
        return result;
    }
};

int main() {
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
            pair<int, int> result = ds.search(val);; 
            int ly = result.first;
            if (ly != -1) {
                cout << "Found\n";
            } else {
                cout << "Not Found\n";
            }
        } else if (op == 'd') {
            if (ds.remove(val)) {
                cout << "Delete Success\n";
            } else {
                cout << "Delete Failed\n";
            }
        } else if (op == 'i') {
            if (ds.insert(val)) {
                cout << "Insert Success\n";
            } else {
                cout << "Insert Failed\n";
            }
        }
    }

    return 0;
}