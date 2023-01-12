#include <vector>
#include <string>


class Solution {
    std::vector<int> result;
    std::vector<bool> visited;

public:
    std::vector<int> countSubTrees(int n, const std::vector<std::vector<int>>& edges, const std::string& labels) {
        result = std::vector<int>(n, 0);

        std::vector<std::vector<int>> graph(n);
        for (const auto& edge : edges) {
            int from = edge[0];
            int to = edge[1];

            graph[from].push_back(to);
            graph[to].push_back(from);
        }

        visited = std::vector<bool>(n, false);
        dfs(0, graph, labels);

        return result;
    }

private:
    std::vector<int> dfs(int i, const std::vector<std::vector<int>>& g, const std::string& labels) {
        std::vector<int> stats(26, 0);

        stats[labels[i] - 'a']++;
        visited[i] = true;

        for (int nei : g[i]) {
            if (visited[nei]) {
                continue;
            }

            auto ch_stats = dfs(nei, g, labels);
            for (int k = 0; k < ch_stats.size(); k++) {
                stats[k] += ch_stats[k];
            }
        }

        result[i] = stats[labels[i] - 'a'];
        return stats;
    }
};