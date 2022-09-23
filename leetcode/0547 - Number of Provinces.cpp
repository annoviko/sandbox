class Solution {
public:
    int findCircleNum(const std::vector<std::vector<int>>& isConnected) {
        std::vector<bool> visited(isConnected.size(), false);
        int result = 0;
        for (std::size_t i = 0; i < isConnected.size(); i++) {
            if (!visited[i]) {
                bfs(i, isConnected, visited);
                result++;
            }
        }

        return result;
    }

    void bfs(const std::size_t i, const std::vector<std::vector<int>>& isConnected, std::vector<bool> & visited) {
        std::queue<std::size_t> queue;
        queue.push(i);

        while(!queue.empty()) {
            std::size_t cur_node = queue.front();
            queue.pop();

            visited[cur_node] = true;

            for (std::size_t j = 0; j < isConnected.size(); j++) {
                if (isConnected[cur_node][j]) {
                    if (!visited[j]) {
                        queue.push(j);
                        visited[j] = true;
                    }
                }
            }
        }
    }
};