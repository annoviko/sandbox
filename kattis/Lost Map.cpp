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
    std::int64_t dist;
};


int main() {
    int n = 0;
    std::cin >> n;

    std::vector<edge> edges;
    for (int i = 0; i < n; i++) {
        int j = 0;
        int temp = 0;

        for (; j < i + 1; j++) {
            std::cin >> temp;
        }

        for (; j < n; j++) {
            int distance = 0;
            std::cin >> distance;
            edges.push_back({ i, j, distance });
        }
    }

    std::sort(edges.begin(), edges.end(), [](const edge& l, const edge& r) {
        return l.dist < r.dist;
    });

    union_find uf(n);
    int counter = 0;

    for (const auto& e : edges) {
        if (uf.is_same_set(e.from, e.to)) {
            continue;
        }

        std::cout << e.from + 1 << ' ' << e.to + 1 << std::endl;

        uf.merge(e.from, e.to);
        counter++;

        if (counter == n - 1) {
            break;
        }
    }

    return 0;
}