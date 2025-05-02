#include <algorithm>
#include <iostream>
#include <vector>


class union_find {
private:
    std::vector<int> p;
    std::vector<int> s;

public:
    union_find(const int n) :
        p(n, 0),
        s(n, 1)
    {
        for (int i = 0; i < n; i++) {
            p[i] = i;
        }
    }

public:
    int get_set_id(const int i) {
        if (p[i] != i) {
            p[i] = get_set_id(p[i]);
        }

        return p[i];
    }


    bool is_same_set(const int i, const int j) {
        return get_set_id(i) == get_set_id(j);
    }


    void merge(const int i, const int j) {
        int sid1 = get_set_id(i);
        int sid2 = get_set_id(j);

        if (sid1 == sid2) {
            return;
        }

        if (s[sid1] < s[sid2]) {
            std::swap(sid1, sid2);
        }

        p[sid2] = sid1;
        s[sid1] += s[sid2];
    }
};


struct edge {
    int from;
    int to;
    std::uint64_t dist;
};


struct node {
    int x;
    int y;
    int z;
};


int main() {
    int n = 0;
    std::cin >> n;

    std::vector<node> p(n);
    std::vector<int> idxs;
    for (int i = 0; i < n; i++) {
        std::cin >> p[i].x >> p[i].y >> p[i].z;
        idxs.push_back(i);
    }

    auto x_comp = [&p](const int& l, const int& r) { return p[l].x < p[r].x; };
    auto y_comp = [&p](const int& l, const int& r) { return p[l].y < p[r].y; };
    auto z_comp = [&p](const int& l, const int& r) { return p[l].z < p[r].z; };

    std::vector<edge> es;

    auto add_edges_func = [&es, &p, &idxs]() {
        for (int i = 1; i < p.size(); i++) {
            int idx1 = idxs[i];
            int idx2 = idxs[i - 1];

            const node& u = p[idx1];
            const node& v = p[idx2];

            std::uint64_t d = std::min(std::min(std::abs(u.x - v.x), std::abs(u.y - v.y)), std::abs(u.z - v.z));
            es.push_back({ idx1, idx2, d });
        }
    };

    std::sort(idxs.begin(), idxs.end(), x_comp);
    add_edges_func();

    std::sort(idxs.begin(), idxs.end(), y_comp);
    add_edges_func();

    std::sort(idxs.begin(), idxs.end(), z_comp);
    add_edges_func();

    std::sort(es.begin(), es.end(), [](const edge& l, const edge& r) { return l.dist < r.dist; });

    union_find uf(n);

    std::uint64_t cost = 0;
    int edge_counter = 0;

    for (const auto& e : es) {
        if (uf.is_same_set(e.from, e.to)) {
            continue;
        }

        uf.merge(e.from, e.to);

        cost += e.dist;
        edge_counter++;

        if (edge_counter == n - 1) {
            break;
        }
    }

    std::cout << cost << std::endl;

    return 0;
}