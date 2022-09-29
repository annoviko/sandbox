class Solution {
public:
    bool canFinish(int numCourses, const std::vector<std::vector<int>>& prerequisites) {
        std::vector<int> counters(numCourses, 0); // memory O(N)
        std::vector<std::vector<int>> graph(numCourses); // memory O(N) -> O(2N)

        for (std::size_t i = 0; i < prerequisites.size(); i++) { // algorithm O(N)
            const int from = prerequisites[i][0];
            const int to = prerequisites[i][1];

            counters[to]++;

            graph[from].push_back(to);
        }

        std::queue<std::size_t> queue;
        for (std::size_t i = 0; i < counters.size(); i++) { // algorithm O(N) -> O(2N)
            if (counters[i] == 0) {
                queue.push(i);
            }
        }

        int courses = 0;
        while (!queue.empty()) { // algorithm O(N) -> O(3N)
            std::size_t node = queue.front();
            queue.pop();

            courses++;

            for (auto idx_nei : graph[node]) { // algorithm O(E) -> O(3N + E)
                counters[idx_nei]--;
                if (counters[idx_nei] == 0) {
                    queue.push(idx_nei);
                }
            }
        }

        return courses == numCourses; // memory: O(N), algorithm: (N + E)
    }
};