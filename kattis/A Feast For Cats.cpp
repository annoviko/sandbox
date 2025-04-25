#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>


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
    int dist;
};


int main() {
    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        int v = 0, milk = 0;
        std::cin >> milk >> v;

        int e_num = v * (v - 1) / 2;
        std::vector<edge> edges;
        for (int j = 0; j < e_num; j++) {
            int cat1, cat2, dist;
            std::cin >> cat1 >> cat2 >> dist;

            edges.push_back({ cat1, cat2, dist });
        }

        std::sort(edges.begin(), edges.end(), [](const edge& l, const edge& r) {
            return l.dist < r.dist;
        });

        union_find uf(v);
        int counter = 0;

        milk -= v;    /* consider that all cats drink milk */

        for (const edge& e : edges) {
            if (uf.is_same_set(e.from, e.to)) {
                continue;
            }

            uf.merge(e.from, e.to);

            milk -= e.dist;
            counter++;

            if ((counter == v - 1) || (milk < 0.0)) {
                break;
            }
        }

        std::cout << ((milk < 0) ? "no" : "yes") << std::endl;
    }

    return 0;
}