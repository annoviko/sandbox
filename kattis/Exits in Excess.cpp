#include <iomanip>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>


using graph = std::vector<std::unordered_map<int, int>>;


void scan_to_remove(const int cur, graph& g, std::vector<bool>& visited, std::unordered_set<int>& direct, std::unordered_set<int>& reverse) {
    visited[cur] = true;

    for (auto iter_nei = g[cur].begin(); iter_nei != g[cur].end(); iter_nei++) {
        int nei = iter_nei->first;
        int edge_id = iter_nei->second;

        if (cur > nei) {
            direct.insert(edge_id);
        }
        else {
            reverse.insert(edge_id);
        }

        if (!visited[nei]) {
            scan_to_remove(nei, g, visited, direct, reverse);
        }
    }
}


int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n, m;
    std::cin >> n >> m;

    graph g(n);

    for (int i = 0; i < m; i++) {
        int from, to;
        std::cin >> from >> to;

        from--;
        to--;

        g[from].insert({ to, i });
    }

    std::unordered_set<int> direct;
    std::unordered_set<int> reverse;
    std::vector<bool> visited(n, false);

    scan_to_remove(0, g, visited, direct, reverse);

    std::unordered_set<int>& edges_to_remove = (direct.size() < reverse.size()) ? direct : reverse;

    std::cout << edges_to_remove.size() << '\n';
    for (int edge_id : edges_to_remove) {
        std::cout << edge_id + 1 << '\n';
    }

    return 0;
}