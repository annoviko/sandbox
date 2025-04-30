#include <iostream>
#include <vector>
#include <queue>


struct entry {
    int w;
    int v;
    int p;
};


struct comparator {
public:
    bool operator()(const entry& l, const entry& r) const {
        return l.w > r.w;
    }
};


int main() {
    int n, m;
    std::cin >> n >> m;

    std::vector<std::vector<std::pair<int, int>>> es(n);
    for (int i = 0; i < m; i++) {
        int v, u, w;
        std::cin >> v >> u >> w;

        es[v].push_back({ u, w });
        es[u].push_back({ v, w });
    }

    std::vector<bool> taken(n);
    std::priority_queue<entry, std::vector<entry>, comparator> pq;

    auto proc_func = [&es, &taken, &pq](const int u) {
        taken[u] = true;
        for (const auto& pair : es[u]) {
            int v = pair.first;
            int w = pair.second;

            if (!taken[v]) {
                pq.push({ w, v, u });
            }
        }
    };

    proc_func(0);

    int edges_taken = 0;
    int longest_mst_edge = 0;

    while (!pq.empty()) {
        const entry e = pq.top();
        pq.pop();

        if (taken[e.v]) {
            continue;
        }

        proc_func(e.v);

        longest_mst_edge = std::max(e.w, longest_mst_edge);
        edges_taken++;

        if (edges_taken == n - 1) {
            break;
        }
    }

    if (edges_taken == n - 1) {
        std::cout << longest_mst_edge << std::endl;
    }
    else {
        std::cout << "IMPOSSIBLE" << std::endl;
    }

    return 0;
};