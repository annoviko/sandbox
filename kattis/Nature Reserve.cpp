#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <queue>


struct edge {
    int v;
    int w;
};


struct comparator {
    bool operator()(const edge& l, const edge& r) {
        return l.w > r.w;
    }
};


int main() {
    int t = 0;
    std::cin >> t;

    for (int _t = 0; _t < t; _t++) {
        int n, m, l, s;
        std::cin >> n >> m >> l >> s;

        std::vector<int> sources;
        std::vector<bool> visited(n, false);
        for (int i = 0; i < s; i++) {
            int id = 0;
            std::cin >> id;

            id--;

            visited[id] = true;
            sources.push_back(id);
        }

        std::vector<std::vector<edge>> g(n);
        for (int i = 0; i < m; i++) {
            int v, u, w;
            std::cin >> v >> u >> w;

            v--;
            u--;

            g[v].push_back({ u, w + l });
            g[u].push_back({ v, w + l });
        }

        std::priority_queue<edge, std::vector<edge>, comparator> pq;

        for (int src_id : sources) {
            for (const edge& e : g[src_id]) {
                pq.push(e);
            }
        }

        std::int64_t cost = 0;

        while (!pq.empty()) {
            edge e = pq.top();
            pq.pop();

            if (visited[e.v]) {
                continue;
            }

            visited[e.v] = true;
            cost += e.w;

            for (const edge& nei : g[e.v]) {
                if (!visited[nei.v]) {
                    pq.push(nei);
                }
            }
        }

        std::cout << cost << std::endl;
    }

    return 0;
}
