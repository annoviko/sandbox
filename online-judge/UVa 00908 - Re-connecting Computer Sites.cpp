#include <regex>
#include <vector>
#include <string>
#include <sstream>
#include <limits>


struct edge {
public:
    int from;
    int to;
    int cost = std::numeric_limits<int>::max();

public:
    edge(int p_from, int p_to, int p_cost) :
        from(p_from),
        to(p_to),
        cost(p_cost)
    { }
};


class union_find {
private:
    std::vector<int> m_p;
    std::vector<int> m_rank;

public:
    union_find(const int N) :
        m_p(N, 0),
        m_rank(N, 1)
    {
        for (int i = 1; i < N; i++) {
            m_p[i] = i;
        }
    }

public:
    int find_set(const int p_index) {
        if (m_p[p_index] == p_index) {
            return p_index;
        }

        m_p[p_index] = find_set(m_p[p_index]);
        return m_p[p_index];
    }


    bool is_same_set(const int p_index1, const int p_index2) {
        return find_set(p_index1) == find_set(p_index2);
    }


    void union_set(const int p_index1, const int p_index2) {
        if (is_same_set(p_index1, p_index2)) {
            return;
        }

        int representer1 = find_set(p_index1);
        int representer2 = find_set(p_index2);

        if (m_rank[representer1] > m_rank[representer2]) {
            m_p[representer2] = representer1;
            m_rank[representer1] += m_rank[representer2];
        }
        else {
            m_p[representer1] = representer2;
            m_rank[representer2] += m_rank[representer1];
        }
    }
};


class solution {
private:
    union_find m_union;
    std::vector<edge> m_edges;
    int m_cost = 0;

public:
    solution(const int n, const std::vector<edge>& p_graph) : 
        m_union(n),
        m_edges(p_graph)
    { }

public:
    int optimize() {
        std::sort(m_edges.begin(), m_edges.end(), [](const edge& a, const edge& b) { return a.cost < b.cost; });

        for (const auto & e : m_edges) {
            if (!m_union.is_same_set(e.from, e.to)) {
                m_cost += e.cost;
                m_union.union_set(e.from, e.to);
            }
        }

        return m_cost;
    }
};



#include <iostream>


int main() {
    int task = 0;
    for (std::string line; std::getline(std::cin, line);) {
        if (line.empty()) {
            continue;
        }


        int n;
        {
            std::stringstream stream(line);
            stream >> n;
        }

        std::vector<edge> edges;
        int cur_cost = 0;

        for (int i = 0; i < n - 1; i++) {
            std::getline(std::cin, line);
            std::stringstream stream(line);

            int from, to, cost;
            stream >> from >> to >> cost;
            cur_cost += cost;
        }

        int k;
        {
            std::getline(std::cin, line);
            std::stringstream stream(line);
            stream >> k;
        }

        for (int i = 0; i < k; i++) {
            std::getline(std::cin, line);
            std::stringstream stream(line);

            int from, to, cost;
            stream >> from >> to >> cost;

            edges.push_back(edge(from - 1, to - 1, cost));
        }

        int m;
        {
            std::getline(std::cin, line);
            std::stringstream stream(line);
            stream >> m;
        }

        for (int i = 0; i < m; i++) {
            std::getline(std::cin, line);
            std::stringstream stream(line);

            int from, to, cost;
            stream >> from >> to >> cost;

            edges.push_back(edge(from - 1, to - 1, cost));
        }

        if (task != 0) {
            std::cout << std::endl;
        }
        std::cout << cur_cost << std::endl;
        std::cout << solution(n, edges).optimize() << std::endl;

        task++;
    }

    return 0;
}