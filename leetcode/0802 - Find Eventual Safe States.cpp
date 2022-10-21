class Solution {
public:
    vector<int> eventualSafeNodes(vector<vector<int>>& graph) {
        std::queue<int> queue;
        std::vector<int> result;
        
        std::vector<std::vector<int>> incoming_conn(graph.size());
        
        std::vector<int> counters(graph.size(), 0);
        for (std::size_t i = 0; i < counters.size(); i++) {
            counters[i] = graph[i].size();
            if (counters[i] == 0) {
                queue.push(i);
                result.push_back(i);
            }
            
            for (int nei : graph[i]) {
                incoming_conn[nei].push_back(i);
            }
        }
        
        while (!queue.empty()) {
            int cur_idx = queue.front();
            queue.pop();
            
            for (int nei : incoming_conn[cur_idx]) {
                counters[nei]--;
                if (counters[nei] == 0) {
                    queue.push(nei);
                    result.push_back(nei);
                }
            }
        }
        
        std::sort(result.begin(), result.end());
        return result;
    }
};