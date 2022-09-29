class Solution {
public:
    std::vector<int> findOrder(int numCourses, const std::vector<std::vector<int>>& prerequisites) {
        std::vector<int> order;

        std::vector<int> counters(numCourses, 0);
        std::vector<std::vector<int>> graph(numCourses);
        for (auto & pair : prerequisites) {
            int from = pair[0];
            int to = pair[1];

            graph[from].push_back(to);
            counters[to]++;
        }

        std::queue<int> queue;
        for (int i = 0; i < numCourses; i++) {
            if (counters[i] == 0) {
                queue.push(i);
            }
        }

        while (!queue.empty()) {
            int cur = queue.front();
            queue.pop();

            order.push_back(cur);

            for (auto idx_nei : graph[cur]) {
                counters[idx_nei]--;
                if (counters[idx_nei] == 0) {
                    queue.push(idx_nei);
                }
            }
        }

        std::reverse(order.begin(), order.end());
        return order.size() == numCourses ? order : std::vector<int>();
    }
};