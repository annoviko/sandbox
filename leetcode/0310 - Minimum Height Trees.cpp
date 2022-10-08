using graph = std::vector<std::vector<int>>;

class Solution {
public:
    std::vector<int> findMinHeightTrees(int n, const std::vector<std::vector<int>>& edges) {
        graph g(n);
        for (auto& edge : edges) {
            int from = edge[0];
            int to = edge[1];

            g[from].push_back(to);
            g[to].push_back(from);
        }

        std::queue<int> cur_queue;

        std::vector<int> counters(n, 0);
        for (int i = 0; i < n; i++) {
            counters[i] = g[i].size();
            if (counters[i] <= 1) {
                cur_queue.push(i);
                counters[i] = 0;
            }
        }

        int remaining = g.size();

        while (remaining != cur_queue.size()) {
            remaining -= cur_queue.size();

            std::queue<int> next_queue;

            while (!cur_queue.empty()) {
                int cur = cur_queue.front();
                cur_queue.pop();

                for (auto& nei : g[cur]) {
                    counters[nei]--;

                    if (counters[nei] == 1) {
                        next_queue.push(nei);
                    }
                }
            }

            cur_queue = std::move(next_queue);
        }

        std::vector<int> result;
        while (!cur_queue.empty()) {
            int idx = cur_queue.front();
            cur_queue.pop();

            result.push_back(idx);
        }

        return result;
    }
};