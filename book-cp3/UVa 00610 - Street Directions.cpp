#include <vector>
#include <string>
#include <sstream>
#include <limits>


using graph = std::vector<std::vector<int>>;
using edge = std::vector<int>;


class Solution {
private:
    graph m_graph;
    std::vector<edge> m_directed_graph;

    std::vector<int> m_ids;
    std::vector<int> m_lowlinks;
    int m_counter = 0;

public:
    std::vector<edge> rebuild_roads(const graph& p_graph) {
        m_graph = p_graph;
        m_ids = std::vector<int>(p_graph.size(), -1);
        m_lowlinks = std::vector<int>(p_graph.size(), std::numeric_limits<int>::max());
        m_counter = 0;

        dfs(0, -1);

        return m_directed_graph;
    }

private:
    void dfs(int p_node, int p_parent) {
        m_counter++;

        m_ids[p_node] = m_counter;
        m_lowlinks[p_node] = m_counter;

        for (auto& nei : m_graph[p_node]) {
            if (m_ids[nei] == -1) {
                dfs(nei, p_node);

                m_lowlinks[p_node] = std::min(m_lowlinks[p_node], m_lowlinks[nei]);

                if (m_lowlinks[nei] > m_ids[p_node]) {
                    /* it is a bridge, we cannot remove it */
                    m_directed_graph.push_back({ nei, p_node });
                    m_directed_graph.push_back({ p_node, nei });
                }
                else {
                    /* use the same direction as dfs */
                    m_directed_graph.push_back({ p_node, nei });
                }
            }
            else if (nei != p_parent) {
                m_lowlinks[p_node] = std::min(m_lowlinks[p_node], m_ids[nei]);

                if (m_ids[p_node] > m_ids[nei]) {
                    /* return back to smallest node */
                    m_directed_graph.push_back({ p_node, nei });
                }
            }
        }
    }
};



#include <iostream>


std::ostream& operator<<(std::ostream& p_stream, const std::vector<int>& p_vec) {
    p_stream << "[ ";
    for (auto elem : p_vec) {
        p_stream << elem << " ";
    }
    p_stream << "]";
    return p_stream;
}


std::ostream& operator<<(std::ostream& p_stream, const std::vector<std::vector<int>>& p_vec) {
    p_stream << "[ ";
    for (auto elem : p_vec) {
        p_stream << elem << " ";
    }
    p_stream << "]";
    return p_stream;
}



int main() {
    int case_num = 1;

    for (std::string line; std::getline(std::cin, line); case_num++) {
        std::stringstream stream(line);

        int n, m;
        stream >> n >> m;

        if (n == 0 && m == 0) {
            break;
        }

        graph g = graph(n);
        for (int i = 0; i < m; i++) {
            std::getline(std::cin, line);
            stream = std::stringstream(line);

            int from, to;
            stream >> from >> to;

            from--;
            to--;

            g[from].push_back(to);
            g[to].push_back(from);
        }

        std::cout << case_num << std::endl << std::endl;

        auto edges = Solution().rebuild_roads(g);
        for (auto& edge : edges) {
            std::cout << edge[0] + 1 << " " << edge[1] + 1 << std::endl;
        }

        std::cout << "#" << std::endl;
    }

    return 0;
}