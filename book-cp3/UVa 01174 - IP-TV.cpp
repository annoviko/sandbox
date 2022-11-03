#include <algorithm>
#include <numeric>
#include <vector>


class disjoin_sets {
private:
    std::vector<int> m_ptr;
    std::vector<int> m_rank;

public:
    disjoin_sets(const int n) :
        m_ptr(n, 0),
        m_rank(n, 1)
    {
        std::iota(m_ptr.begin(), m_ptr.end(), 0);
    }

public:
    int find_set(const int p_index) {
        if (p_index != m_ptr[p_index]) {
            m_ptr[p_index] = find_set(m_ptr[p_index]);
        }

        return m_ptr[p_index];
    }

    bool is_same_set(const int p_index1, const int p_index2) {
        const int set_id1 = find_set(p_index1);
        const int set_id2 = find_set(p_index2);

        return set_id1 == set_id2;
    }

    void merge(const int p_index1, const int p_index2) {
        const int set_id1 = find_set(p_index1);
        const int set_id2 = find_set(p_index2);

        if (set_id1 == set_id2) {
            return;
        }

        if (m_rank[set_id1] > m_rank[set_id2]) {
            m_ptr[set_id2] = set_id1;
            m_rank[set_id1] += m_rank[set_id2];
        }
        else {
            m_ptr[set_id1] = set_id2;
            m_rank[set_id2] += m_rank[set_id1];
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
    disjoin_sets m_sets;

public:
    solution(const int n, const graph p_graph) :
        m_graph(p_graph),
        m_sets(n)
    { }

public:
    int optimize() {
        int cost = 0;

        std::sort(m_graph.begin(), m_graph.end(), [](const edge& l, const edge& r) {
            return l.cost < r.cost;
        });

        for (auto& edge : m_graph) {
            if (m_sets.is_same_set(edge.from, edge.to)) {
                continue;
            }

            cost += edge.cost;
            m_sets.merge(edge.from, edge.to);
        }

        return cost;
    }
};


#include <iostream>
#include <string>
#include <unordered_map>

int main() {
    int tasks = 0;
    std::cin >> tasks;

    for (int i = 0; i < tasks; i++) {
        int n, m;
        std::cin >> n >> m;

        int idx_free = 0;
        std::unordered_map<std::string, int> idx_map;

        graph g;

        for (int i = 0; i < m; i++) {
            std::string str_from, str_to;
            int cost;

            std::cin >> str_from >> str_to >> cost;

            int from, to;

            auto iter = idx_map.find(str_from);
            if (iter == idx_map.end()) {
                from = idx_free;
                idx_map[str_from] = idx_free;
                idx_free++;
            }
            else {
                from = iter->second;
            }

            iter = idx_map.find(str_to);
            if (iter == idx_map.end()) {
                to = idx_free;
                idx_map[str_to] = idx_free;
                idx_free++;
            }
            else {
                to = iter->second;
            }

            g.push_back(edge(from, to, cost));
        }

        if (i > 0) {
            std::cout << std::endl;
        }

        std::cout << solution(n, g).optimize() << std::endl;
    }

    return 0;
}