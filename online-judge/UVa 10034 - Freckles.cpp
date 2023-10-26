#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>


class disjoint_sets {
private:
    std::vector<int> m_ids;
    std::vector<int> m_ranks;

public:
    disjoint_sets(const int m_size) :
        m_ids(m_size, 0),
        m_ranks(m_size, 1)
    {
        std::iota(m_ids.begin(), m_ids.end(), 0);
    }

    int get_set_id(const int p_id) {
        if (m_ids[p_id] != p_id) {
            m_ids[p_id] = get_set_id(m_ids[p_id]);
        }

        return m_ids[p_id];
    }

    bool is_same_set(const int p_id1, const int p_id2) {
        const int set_id1 = get_set_id(p_id1);
        const int set_id2 = get_set_id(p_id2);

        return set_id1 == set_id2;
    }

    void merge(const int p_id1, const int p_id2) {
        const int set_id1 = get_set_id(p_id1);
        const int set_id2 = get_set_id(p_id2);

        if (m_ranks[set_id1] > m_ranks[set_id2]) {
            m_ranks[set_id1] += m_ranks[set_id2];
            m_ids[set_id2] = set_id1;
        }
        else {
            m_ranks[set_id2] += m_ranks[set_id1];
            m_ids[set_id1] = set_id2;
        }
    }
};


struct edge {
    int from = -1;
    int to = -1;
    double distance = 0.0;

public:
    edge(int p_from, int p_to, double p_distance) :
        from(p_from),
        to(p_to),
        distance(p_distance)
    { }
};


class mst {
public:
    double calculate(const std::vector<std::pair<double, double>>& p_coord) {
        std::vector<edge> edges;
        for (std::size_t i = 0; i < p_coord.size(); i++) {
            for (std::size_t j = i + 1; j < p_coord.size(); j++) {
                const double distance = std::sqrt(std::pow(p_coord[i].first - p_coord[j].first, 2) + std::pow(p_coord[i].second - p_coord[j].second, 2));
                edges.push_back(edge(i, j, distance));
            }
        }

        std::sort(edges.begin(), edges.end(), [](const edge& p1, const edge& p2) { return p1.distance < p2.distance; });

        double distance = 0.0;
        disjoint_sets sets(p_coord.size());

        for (const edge& e : edges) {
            if (sets.is_same_set(e.from, e.to)) {
                continue;
            }

            distance += e.distance;
            sets.merge(e.from, e.to);
        }

        return distance;
    }
};


#include <iostream>
#include <iomanip>


int main() {
    int cases;
    std::cin >> cases;

    for (int i = 0; i < cases; i++) {
        int n;
        std::cin >> n;

        std::vector<std::pair<double, double>> coord;

        for (int i = 0; i < n; i++) {
            
            double c1, c2;
            std::cin >> c1 >> c2;

            coord.push_back(std::pair<double, double>(c1, c2));
        }

        const double distance = mst().calculate(coord);
        std::cout << std::setprecision(2) << std::fixed << distance << std::endl;
        if (i != cases - 1) {
            std::cout << std::endl;
        }
    }

    return 0;
}