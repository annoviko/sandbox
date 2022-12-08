#include <iostream>
#include <queue>

// Birthday Party
// 2014 Virginia Tech High School Programming Contest


using graph = std::vector<std::vector<int>>;


class solution {
private:
    graph m_graph;

    std::vector<int> m_ids;
    std::vector<int> m_lowlinks;

    int m_counter = 0;
    int m_bridges = 0;

public:
    solution(const graph& p_graph) :
        m_graph(p_graph),
        m_ids(p_graph.size(), -1),
        m_lowlinks(p_graph.size(), -1),
        m_counter(0),
        m_bridges(0)
    { }

public:
    bool has_critical_links() {
        for (int i = 0; i < (int) m_graph.size(); i++) {
            if (m_ids[i] == -1) {
                dfs(i, -1);
            }
        }

        return m_bridges > 0;
    }

private:
    void dfs(const int p_node, const int p_parent) {
        m_counter++;

        m_ids[p_node] = m_counter;
        m_lowlinks[p_node] = m_counter;

        for (int nei : m_graph[p_node]) {
            if (m_ids[nei] == -1) {
                dfs(nei, p_node);

                m_lowlinks[p_node] = std::min(m_lowlinks[p_node], m_lowlinks[nei]);
                if (m_lowlinks[nei] > m_ids[p_node]) {
                    m_bridges++;
                }
            }
            else if (nei != p_parent) {
                m_lowlinks[p_node] = std::min(m_lowlinks[p_node], m_ids[nei]);
            }
        }
    }
};


int main() {
    while (true) {
        int p, c;
        std::cin >> p >> c;

        if ((p == 0) && (c == 0)) {
            return 0;
        }

        graph g(p);

        for (int i = 0; i < c; i++) {
            int from, to;
            std::cin >> from >> to;

            g[from].push_back(to);
            g[to].push_back(from);
        }

        std::cout << (solution(g).has_critical_links() ? "yes" : "no") << std::endl;
    }

    return 0;
};

