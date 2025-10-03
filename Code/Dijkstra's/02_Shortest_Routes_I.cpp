#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <string>
#include <fstream>

const long long INF = LLONG_MAX;

using namespace std;

void dijkstra(vector<vector<pair<int, long long>>> &graph,  vector<long long>& dist, int n) {
    // In the priority queue I want to keep the distance as first value of the pair
    // That is, now the pair is like (w, b), where w is the weight and b is the node
    // Doing this I assure the distances is considered first for my priority queue 
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>> pq;
    
    dist[1] = 0; 
    pq.push({0, 1});

    while (!pq.empty()) {
        long long d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue; // d is outdated process the next node. 

        for (auto &edge : graph[u]) {
            int v = edge.first;
            long long weight = edge.second;

            // RELAX
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }
}

int main() {
    // --> For one or multiple files execution
    for (int file_num = 1; file_num <= 6; file_num++) {
        string filename = "tests2/" + to_string(file_num) + ".in";
        ifstream infile(filename);
        if (!infile) {
            return 1;
        }

        int n, m;
        infile >> n >> m; 
        
        vector<vector<pair<int, long long>>> graph(n + 1);
        for (int i = 0; i < m; i++) {
            int a, b;
            long long c;
            infile >> a >> b >> c;
            graph[a].push_back({b, c});
        }
        infile.close();

        vector<long long> dist(n + 1, INF); // All distances initialized to INF

        dijkstra(graph, dist, n); // Single-Source Shortest-Paths

        
        string out_filename = "outputs2/" + to_string(file_num) + ".out"; 
        ofstream outfile (out_filename);
        for (int i = 1; i <= n; i++) {
            outfile << dist[i] << " ";
        }
        outfile << endl;
        outfile.close();
    }

    
    return 0;
}
