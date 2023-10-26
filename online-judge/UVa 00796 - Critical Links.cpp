#include <regex>
#include <vector>
#include <string>
#include <sstream>
#include <limits>


using graph = std::vector<std::vector<int>>;


class Solution {
private:
    graph m_graph;

    std::vector<std::pair<int, int>> m_bridges;

    int m_counter = 0;
    std::vector<int> m_ids;
    std::vector<int> m_lowlinks;

public:
    Solution(const graph& p_graph) : 
        m_graph(p_graph),
        m_counter(0),
        m_ids(p_graph.size(), -1),
        m_lowlinks(p_graph.size(), std::numeric_limits<int>::max())
    { }

public:
    std::vector<std::pair<int, int>> count_critical_links() {
        for (int i = 0; i < m_graph.size(); i++) {
            if (m_ids[i] == -1) {
                dfs(i, -1);
            }
        }

        return m_bridges;
    }

    void dfs(int p_node, int p_parent) {
        m_counter++;

        m_ids[p_node] = m_counter;
        m_lowlinks[p_node] = m_counter;

        for (auto& nei : m_graph[p_node]) {
            if (m_ids[nei] == -1) {
                dfs(nei, p_node);
                m_lowlinks[p_node] = std::min(m_lowlinks[p_node], m_lowlinks[nei]);

                if (m_lowlinks[nei] > m_ids[p_node]) {
                    if (p_node < nei) {
                        m_bridges.push_back({ p_node, nei });
                    }
                    else {
                        m_bridges.push_back({ nei, p_node });
                    }
                }
            }
            else if (nei != p_parent) {
                m_lowlinks[p_node] = std::min(m_lowlinks[p_node], m_ids[nei]);
            }
        }
    }
};



#include <iostream>


int main() {
    for (std::string line; std::getline(std::cin, line);) {
        if (line.empty()) {
            continue;
        }

        std::stringstream stream(line);

        int n;
        stream >> n;

        graph g = graph(n);
        for (int i = 0; i < n; i++) {
            std::getline(std::cin, line);

            std::regex pattern("(\\d+) \\((\\d+)\\) (.*)");
            std::smatch matches;
            if (std::regex_search(line, matches, pattern)) {
                int from = std::stoi(matches[1].str());
                int nei_num = std::stoi(matches[2].str());
                line = matches[3].str();

                stream = std::stringstream(line);

                for (int i = 0; i < nei_num; i++) {
                    int to;
                    stream >> to;

                    g[from].push_back(to);
                }
            }
        }

        auto edges = Solution(g).count_critical_links();
        std::sort(edges.begin(), edges.end());
        std::cout << edges.size() << " critical links" << std::endl;
        for (auto& edge : edges) {
            std::cout << edge.first << " - " << edge.second << std::endl;
        }

        std::cout << std::endl;
    }

    return 0;
}