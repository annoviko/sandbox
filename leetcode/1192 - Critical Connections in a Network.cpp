using graph = std::vector<std::vector<int>>;


class Solution {
private:
    graph m_graph;
    
    std::vector<int> m_ids;
    std::vector<int> m_lowlinks;
    
    int counter = 0;
    
public:
    vector<vector<int>> criticalConnections(int n, vector<vector<int>>& connections) {
        m_graph = graph(n);
        for (auto & conn : connections) {
            int from = conn[0];
            int to = conn[1];
            
            m_graph[from].push_back(to);
            m_graph[to].push_back(from);
        }
        
        counter = 0;
        m_ids = std::vector<int>(n, -1);
        m_lowlinks = std::vector<int>(n, INT_MAX);
        
        dfs(0, -1);
        
        vector<vector<int>> result;
        for (auto & conn : connections) {
            int from = conn[0];
            int to = conn[1];
            
            if (m_lowlinks[from] > m_ids[to] || m_lowlinks[to] > m_ids[from]) {
                result.push_back({ from, to });
            }
        }
        
        return result;
    }
    
    
    void dfs(int p_node, int p_parent) {
        counter++;
        
        m_ids[p_node] = counter;
        m_lowlinks[p_node] = counter;
        
        for (auto & nei : m_graph[p_node]) {
            if (m_ids[nei] == -1) {
                dfs(nei, p_node);
                m_lowlinks[p_node] = std::min(m_lowlinks[p_node], m_lowlinks[nei]);
            }
            else if (nei != p_parent) {
                m_lowlinks[p_node] = std::min(m_lowlinks[p_node], m_ids[nei]);
            }
        }
    }
};
