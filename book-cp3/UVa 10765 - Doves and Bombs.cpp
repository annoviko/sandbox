#include <algorithm>
#include <set>
#include <vector>


struct answer {
    int m_id = -1;
    int m_graphs = -1;

    answer(int p_id) {
        m_id = p_id;
    }

    answer(int p_id, int p_graphs) {
        m_id = p_id;
        m_graphs = p_graphs;
    }
};


using graph = std::vector<std::vector<int>>;


class solution {
private:
    constexpr static int NOT_VISITED = -1;

private:
    std::vector<int> m_ids;
    std::vector<int> m_lowlinks;
    std::vector<int> m_parents;
    std::vector<int> m_articulations;

    int m_cur_root = 0;
    int m_root_children = 0;
    int m_counter = 0;

    graph m_g;

    std::vector<answer> m_result;

public:
    solution(const graph& p_g) :
        m_g(p_g),
        m_ids(p_g.size(), NOT_VISITED),
        m_lowlinks(p_g.size(), NOT_VISITED),
        m_parents(p_g.size(), NOT_VISITED),
        m_articulations(p_g.size(), 0)
    { }

public:
    solution& run() {
        for (int i = 0; i < m_g.size(); i++) {
            if (m_ids[i] == NOT_VISITED) {
                m_cur_root = i;
                m_root_children = 0;

                find_articulations(i);

                m_articulations[m_cur_root] = (m_root_children > 1);
            }
        }

        for (int i = 0; i < m_articulations.size(); i++) {
            m_articulations[i]++;
            m_result.push_back(answer(i, m_articulations[i]));
        }

        std::sort(m_result.begin(), m_result.end(), [](const answer& l, const answer& r) {
            if (l.m_graphs > r.m_graphs) {
                return true;
            }
            else if (l.m_graphs == r.m_graphs) {
                return l.m_id < r.m_id;
            }

            return false;
            });

        return *this;
    }

    std::vector<answer>& result() {
        return m_result;
    }

private:
    void find_articulations(const int i) {
        m_ids[i] = m_counter;
        m_lowlinks[i] = m_counter;

        m_counter++;

        for (int nei : m_g[i]) {
            if (m_ids[nei] == NOT_VISITED) {
                m_parents[nei] = i;

                if (i == m_cur_root) {
                    m_root_children++;
                }

                find_articulations(nei);

                if (m_lowlinks[nei] >= m_ids[i]) {
                    m_articulations[i]++;
                }

                m_lowlinks[i] = std::min(m_lowlinks[i], m_lowlinks[nei]);
            }
            else if (m_parents[i] != nei) {
                m_lowlinks[i] = std::min(m_lowlinks[i], m_ids[nei]);
            }
        }
    }
};


#include <iostream>


int main() {
    while (true) {
        int n, m;
        std::cin >> n >> m;

        if (m == 0 && n == 0) {
            break;
        }

        graph g(n);

        while (true) {
            int from, to;
            std::cin >> from >> to;

            if (from == -1 && to == -1) {
                break;
            }

            g[from].push_back(to);
            g[to].push_back(from);
        }

        std::vector<answer> result = solution(g).run().result();

        for (int i = 0; i < result.size() && i < m; i++) {
            auto& ans = result[i];
            std::cout << ans.m_id << " " << ans.m_graphs << std::endl;
        }
        std::cout << std::endl;
    }

    return 0;
}
