#include <vector>


class Solution {
    std::vector<bool> visited;

public:
    int minTime(int n, const std::vector<std::vector<int>>& edges, const std::vector<bool>& hasApple) {
        std::vector<std::vector<int>> g(n);
        for (auto& edge : edges) {
            int from = edge[0];
            int to = edge[1];

            g[from].push_back(to);
            g[to].push_back(from);
        }

        visited = std::vector<bool>(n, false);

        return dfs(0, g, hasApple, 0);
    }

    int dfs(int i, const std::vector<std::vector<int>>& g, const std::vector<bool>& hasApple, int depth) {
        int distance = 0;
        if (hasApple[i]) {
            distance = depth;
        }

        visited[i] = true;

        for (int nei : g[i]) {
            if (visited[nei]) {
                continue;
            }

            distance += dfs(nei, g, hasApple, (distance > 0) ? 1 : depth + 1);
        }

        if (i == 0) {
            return distance;
        }

        return (distance > 0) ? (distance + 1) : 0;
    }
};