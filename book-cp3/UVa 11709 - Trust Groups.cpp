#include <iostream>
#include <stack>
#include <vector>
#include <unordered_map>
#include <limits>

using graph = std::vector<std::vector<int>>;


class Solution {
private:
    graph m_graph;

    int m_counter = 0;
    int m_ssc = 0;
    std::vector<int> m_ids;
    std::vector<int> m_lowlinks;

    std::stack<int> m_stack;
    std::vector<bool> m_in_stack;

public:
    Solution(const graph & p_graph) :
        m_graph(p_graph),
        m_ids(p_graph.size(), -1),
        m_lowlinks(p_graph.size(), std::numeric_limits<int>::max()),
        m_in_stack(p_graph.size(), false)
    { }

public:
    int count_groups() {
        for (int i = 0; i < (int) m_ids.size(); i++) {
            if (m_ids[i] == -1) {
                dfs(i);
            }
        }

        return m_ssc;
    }

private:
    void dfs(int p_node) {
        m_counter++;

        m_lowlinks[p_node] = m_counter;
        m_ids[p_node] = m_counter;
        m_in_stack[p_node] = true;
        m_stack.push(p_node);

        for (int nei : m_graph[p_node]) {
            if (m_ids[nei] == -1) {
                dfs(nei);
            }

            if (m_in_stack[nei]) {
                m_lowlinks[p_node] = std::min(m_lowlinks[p_node], m_lowlinks[nei]);
            }
        }

        if (m_lowlinks[p_node] == m_ids[p_node]) {
            m_ssc++;

            while(true) {
                int idx = m_stack.top();
                m_stack.pop();
                m_in_stack[idx] = false;

                if (idx == p_node) {
                    break;
                }
            }
        }
    }
};


#include <string>
#include <sstream>

int main() {
    for (std::string line; std::getline(std::cin, line);) {
        std::stringstream stream(line);
        int nodes = -1, blocks = -1;
        stream >> nodes >> blocks;

        if (nodes + blocks == 0) {
            return 0;
        }

        graph g(nodes);
        std::unordered_map<std::string, int> name_map;
        for (int i = 0; i < nodes; i++) {
            std::getline(std::cin, line);
            name_map[line] = i;
        }

        for (int i = 0; i < blocks; i++) {
            std::string from, to;
            std::getline(std::cin, from);
            std::getline(std::cin, to);

            const int from_idx = name_map[from];
            const int to_idx = name_map[to];

            g[from_idx].push_back(to_idx);
        }

        int count_scc = Solution(g).count_groups();
        std::cout << count_scc << std::endl;
    }

    return 0;
}
