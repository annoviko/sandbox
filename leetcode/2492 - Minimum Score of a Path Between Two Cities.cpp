#include <vector>
#include <queue>


class Solution {
public:
    int minScore(int n, const std::vector<std::vector<int>>& roads) {
        std::vector<std::vector<std::pair<int, int>>> g(n);

        for (const auto& r : roads) {
            const int from = r[0] - 1;
            const int to = r[1] - 1;
            const int w = r[2];

            g[from].push_back({ to, w });
            g[to].push_back({ from , w });
        }

        std::vector<bool> visited(n, false);
        std::queue<int> q;

        q.push(0);
        visited[0] = true;

        int min_len = INT_MAX;
        
        while (!q.empty()) {
            int cur = q.front();
            q.pop();

            for (const auto& to_nei : g[cur]) {
                min_len = std::min(min_len, to_nei.second);
                if (visited[to_nei.first]) {
                    continue;
                }

                q.push(to_nei.first);
                visited[to_nei.first] = true;
            }
        }

        return min_len;
    }
};