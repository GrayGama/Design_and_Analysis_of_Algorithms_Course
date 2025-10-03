#include <iostream>
#include <vector>
#include <set>
#include <string>

using namespace std;

// Detail explanation in the report
int main() {
    int n;
    cin >> n;

    set<int> S;     // Making use of std::set as it creates a balanced BST.
    for (int i = 0; i < n; i++) {
        int data;
        cin >> data;
        S.insert(data);
    }

    int m;
    cin >> m;

    vector<pair<char, int>> cmds(m);
    for (int i = 0; i < m; ++i) {
        cin >> cmds[i].first >> cmds[i].second;
    }

    for (const auto& cmd : cmds){
        char op = cmd.first;
        int val = cmd.second;

        if (op == 's') {
            if (S.find(val) != S.end()) {
                cout << "Found" << endl;
            } else {
                cout << "Not Found" << endl;
            }
        }
        else if (op == 'd') {
            auto it = S.find(val);
            if (it != S.end()) {
                S.erase(it);
                cout << "Delete Success" << endl;
            } else {
                cout << "Delete Failed" << endl;
            }
        }
        else if (op == 'i') {
            auto result = S.insert(val);
            if (result.second) {
                cout << "Insert Success\n";
            } else {
                cout << "Insert Failed\n";
            }
        }
    }

    return 0;
}