#include <algorithm>
#include <vector>
#include <iostream>
#include <queue>
#include <iomanip>
#include <cmath>


class union_find {
private:
    std::vector<int> parent;
    std::vector<int> sizes;

public:
    union_find(int n) :
        parent(n, 0),
        sizes(n, 1)
    {
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

public:
    int get_set_id(int v) {
        if (parent[v] != v) {
            parent[v] = get_set_id(parent[v]);
        }

        return parent[v];
    }

    bool is_same_set(int v, int u) {
        int id1 = get_set_id(v);
        int id2 = get_set_id(u);

        return id1 == id2;
    }

    void merge(int v, int u) {
        int id1 = get_set_id(v);
        int id2 = get_set_id(u);

        if (id1 == id2) {
            return;
        }

        if (sizes[id1] < sizes[id2]) {
            std::swap(id1, id2);
        }

        parent[id2] = id1;
        sizes[id1] += sizes[id2];
    }
};


struct circle {
    int x;
    int y;
    int r;
};


struct edge {
    int v;
    int u;
    long double w;
};


int main() {
    int n = 0;
    std::cin >> n;

    union_find uf(n);
    std::vector<circle> cls(n);

    for (int i = 0; i < n; i++) {
        std::cin >> cls[i].x >> cls[i].y >> cls[i].r;
    }

    std::vector<edge> es;
    for (int i = 0; i < cls.size(); i++) {
        for (int j = i + 1; j < cls.size(); j++) {
            long double distance = std::sqrt(std::pow(cls[i].x - cls[j].x, 2) + std::pow(cls[i].y - cls[j].y, 2));
            distance -= (cls[i].r + cls[j].r);

            es.push_back({ i, j, distance });
        }
    }

    std::sort(es.begin(), es.end(), [](const edge& l, const edge& r) {
        return l.w < r.w;
    });

    long double cost = 0;
    for (const auto& e : es) {
        if (uf.is_same_set(e.u, e.v)) {
            continue;
        }

        uf.merge(e.u, e.v);

        if (e.w > 0) {
            cost += e.w;
        }
    }

    std::cout << std::setprecision(15) << cost << std::endl;

    return 0;
}
