#include <vector>
#include <climits>
#include <stack>


using graph = std::vector<std::vector<int>>;


class Solution {
private:
    int dfs_counter = 0;
    int scc_counter = 0;
    std::vector<int> m_dfs_num;
    std::vector<int> m_dfs_low;
    std::vector<bool> m_visited;
    std::stack<int> m_stack;
    graph m_graph;

public:
    Solution(const graph& p_graph) :
        m_dfs_num(p_graph.size(), -1),
        m_dfs_low(p_graph.size(), INT_MAX),
        m_visited(p_graph.size(), false),
        m_graph(p_graph)
    { }

public:
    int count_scc() {
        for (int i = 0; i < m_graph.size(); i++) {
            if (m_dfs_num[i] == -1) {
                count_scc_dfs(i);
            }
        }

        return scc_counter;
    }

private:
    void count_scc_dfs(int p_node) {
        dfs_counter++;

        m_dfs_num[p_node] = dfs_counter;
        m_dfs_low[p_node] = dfs_counter;
        m_visited[p_node] = true;
        m_stack.push(p_node);

        auto& neighbors = m_graph[p_node];
        for (int nei : neighbors) {
            if (m_dfs_num[nei] == -1) {
                count_scc_dfs(nei);
            }

            if (m_visited[nei]) {
                m_dfs_low[p_node] = std::min(m_dfs_low[p_node], m_dfs_low[nei]);
            }
        }

        if (m_dfs_num[p_node] == m_dfs_low[p_node]) {
            scc_counter++;

            while (true) {
                int idx = m_stack.top();
                m_stack.pop();
                m_visited[idx] = false;

                if (idx == p_node) { 
                    break;
                }
            }
        }
    }
};


#include <iostream>
#include <string>
#include <sstream>


void test() {
    graph input1 = {
        { 1 }, // 0 -> 1
        { 3 }, // 1 -> 3
        { 1 }, // 2 -> 1
        { 2, 4 }, // 3 -> 2, 4
        { 5 }, // 4 -> 5
        { 7 }, // 5 -> 7
        { 4 }, // 6 -> 4
        { 6 }  // 7 -> 6
    };

    graph input2 = {
        { 1, 2 },
        { 0 },
        { }
    };

    graph input3 = {
        { 1, 2, 3 },
        { 3 },
        { 0, 3 },
        { 0 },
        { 5 },
        { 4 }
    };

    std::cout << "SCC: " << Solution(input3).count_scc() << std::endl;
}


int main() {
    for (std::string line; std::getline(std::cin, line);) {
        std::stringstream stream(line);
        int nodes = -1, entries = -1;
        stream >> nodes >> entries;

        if (nodes + entries == 0) {
            return 0;
        }

        graph g(nodes);
        for (int i = 0; i < entries; i++) {
            std::getline(std::cin, line);
            stream = std::stringstream(line);

            int from, to, type;
            stream >> from >> to >> type;

            from--;
            to--;

            g[from].push_back(to);
            if (type == 2) {
                g[to].push_back(from);
            }
        }

        int count_scc = Solution(g).count_scc();
        std::cout << ((count_scc == 1) ? 1 : 0) << std::endl;
    }

    return 0;
}
