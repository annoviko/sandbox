#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>


class union_find {
private:
    std::vector<int> parents;
    std::vector<int> sizes;

public:
    union_find(int set_size) :
        parents(set_size, 0),
        sizes(set_size, 0)
    {
        for (int i = 0; i < set_size; i++) {
            parents[i] = i;
        }
    }

    int get_set_id(int id) {
        if (parents[id] != id) {
            parents[id] = get_set_id(parents[id]);
        }

        return parents[id];
    }

    bool is_same_set(int id1, int id2) {
        int set_id1 = get_set_id(id1);
        int set_id2 = get_set_id(id2);

        return set_id1 == set_id2;
    }

    void merge(int id1, int id2) {
        int set_id1 = get_set_id(id1);
        int set_id2 = get_set_id(id2);

        if (set_id1 == set_id2) {
            return;
        }

        if (sizes[set_id1] < sizes[set_id2]) {
            std::swap(set_id1, set_id2);
        }

        parents[set_id2] = set_id1;
        sizes[set_id1] += sizes[set_id2];
    }
};


struct edge {
    int from;
    int to;
    long double dist;
};


struct position {
    long double x;
    long double y;
};


int main() {
    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        int v = 0;
        std::cin >> v;

        std::vector<position> pos(v);
        for (int j = 0; j < v; j++) {
            std::cin >> pos[j].x >> pos[j].y;
        }

        std::vector<edge> edges;
        for (int j = 0; j < pos.size(); j++) {
            for (int k = j + 1; k < pos.size(); k++) {
                auto p1 = pos[j];
                auto p2 = pos[k];
                long double dist = std::sqrt(std::pow(p2.x - p1.x, 2) + std::pow(p2.y - p1.y, 2));

                edges.push_back({ j, k, dist });
            }
        }

        std::sort(edges.begin(), edges.end(), [](const edge& l, const edge& r) {
            return l.dist < r.dist;
        });

        union_find uf(v);
        int counter = 0;
        long double min_dist = 0.0;

        for (const edge e : edges) {
            if (uf.is_same_set(e.from, e.to)) {
                continue;
            }

            uf.merge(e.from, e.to);
            min_dist += e.dist;
            counter++;

            if (counter == v - 1) {
                break;
            }
        }

        std::cout << std::setprecision(15) << min_dist << std::endl;
    }

    return 0;
}