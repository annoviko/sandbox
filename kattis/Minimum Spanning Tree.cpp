#include <algorithm>
#include <vector>
#include <iostream>
#include <queue>


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


struct edge {
    int v;
    int u;
    int w;
};


int main() {
    while (true) {
        int n, m;
        std::cin >> n >> m;

        if (n == 0 && m == 0) {
            break;
        }

        std::vector<edge> es(m);
        for (int i = 0; i < m; i++) {
            std::cin >> es[i].v >> es[i].u >> es[i].w;
            if (es[i].v > es[i].u) {
                std::swap(es[i].v, es[i].u);
            }
        }

        std::sort(es.begin(), es.end(), [](const edge& e1, const edge& e2) {
            return e1.w < e2.w;
        });

        std::int64_t cost = 0;
        std::vector<std::pair<int, int>> mst;

        union_find uf(n);

        for (const auto& e : es) {
            if (uf.is_same_set(e.v, e.u)) {
                continue;
            }

            uf.merge(e.v, e.u);

            cost += e.w;
            mst.push_back({ e.v, e.u });

            if (mst.size() == n - 1) {
                break;
            }
        }

        if (mst.size() != n - 1) {
            std::cout << "Impossible" << std::endl;
            continue;
        }

        std::sort(mst.begin(), mst.end(), [](const auto& e1, const auto& e2) {
            if (e1.first == e2.first) {
                return e1.second < e2.second;
            }

            return e1.first < e2.first;
        });

        std::cout << cost << std::endl;
        for (const auto& e : mst) {
            std::cout << e.first << ' ' << e.second << std::endl;
        }
    }

    return 0;
}