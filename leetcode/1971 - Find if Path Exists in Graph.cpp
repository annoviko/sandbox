#include <vector>
#include <queue>


using graph = std::vector<std::vector<int>>;


class Solution {
public:
    bool validPath(int n, const std::vector<std::vector<int>>& edges, int source, int destination) {
        graph g(n);

        for (const auto& edge : edges) {
            g[edge[0]].push_back(edge[1]);
            g[edge[1]].push_back(edge[0]);
        }

        std::queue<int> to_process;
        to_process.push(source);

        std::vector<bool> visited(n, false);
        visited[source] = true;

        while (!to_process.empty()) {
            const int cur = to_process.front();
            to_process.pop();

            if (cur == destination) {
                return true;
            }

            for (const int nei : g[cur]) {
                if (visited[nei]) {
                    continue;
                }

                to_process.push(nei);
                visited[nei] = true;
            }
        }

        return false;
    }
};