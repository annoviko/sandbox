#include <algorithm>
#include <numeric>
#include <vector>


class disjoint_sets {
private:
    std::vector<int> m_ptr;
    std::vector<int> m_rank;

public:
    disjoint_sets(const int n) :
        m_ptr(n, 0),
        m_rank(n, 1)
    {
        std::iota(m_ptr.begin(), m_ptr.end(), 0);
    }

public:
    int find_set(const int p_idx) {
        if (m_ptr[p_idx] != p_idx) {
            m_ptr[p_idx] = find_set(m_ptr[p_idx]);
        }

        return m_ptr[p_idx];
    }

    bool is_same_set(const int p_idx1, const int p_idx2) {
        const int set_id1 = find_set(p_idx1);
        const int set_id2 = find_set(p_idx2);

        return set_id1 == set_id2;
    }

    void merge(const int p_idx1, const int p_idx2) {
        const int set_id1 = find_set(p_idx1);
        const int set_id2 = find_set(p_idx2);

        if (set_id1 == set_id2) {
            return;
        }

        if (m_rank[set_id1] > m_rank[set_id2]) {
            m_rank[set_id1] += m_rank[set_id2];
            m_ptr[set_id2] = set_id1;
        }
        else {
            m_rank[set_id2] += m_rank[set_id1];
            m_ptr[set_id1] = set_id2;
        }
    }
};


struct edge {
public:
    int from;
    int to;
    int cost;

public:
    edge(const int p_from, const int p_to, const int p_cost) :
        from(p_from), to(p_to), cost(p_cost)
    { }
};


using graph = std::vector<edge>;


class solution {
private:
    graph m_graph;
    graph m_mst;
    disjoint_sets m_sets;

public:
    solution(const int n, const graph& p_graph) :
        m_graph(p_graph),
        m_sets(n)
    { }

public:
    solution& optimize() {
        std::sort(m_graph.begin(), m_graph.end(), [](const edge& l, const edge& r) {
            return l.cost < r.cost;
        });

        for (auto & edge : m_graph) {
            if (m_sets.is_same_set(edge.from, edge.to)) {
                continue;
            }

            m_sets.merge(edge.from, edge.to);
            m_mst.push_back(edge);
        }

        return *this;
    }

    const graph& result() const {
        return m_mst;
    }
};


#include <iostream>
#include <string>
#include <sstream>


int main() {
    int tasks = 0;

    std::string line;
    std::getline(std::cin, line);
    std::stringstream stream(line);
    stream >> tasks;

    for (int i = 0; i < tasks; i++) {
        int n;

        std::getline(std::cin, line);
        stream = std::stringstream(line);
        stream >> n;

        graph g;

        for (int row = 0; row < n; row++) {
            std::getline(std::cin, line);
            stream = std::stringstream(line);

            for (int col = 0; col < n; col++) {
                if (stream.peek() == ',') {
                    stream.ignore(2);
                }

                int guards;
                stream >> guards;

                if ((col > row) && (guards != 0)) {
                    g.push_back(edge(row, col, guards));
                }
            }
        }

        graph mst = solution(n, g).optimize().result();

        std::cout << "Case " << i + 1 << ":" << std::endl;
        for (const edge& e : mst) {
            std::cout << (unsigned char) (e.from + 'A') << '-' << (unsigned char) (e.to + 'A') << ' ' << e.cost << std::endl;
        }
    }

    return 0;
}