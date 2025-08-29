#include <vector>
#include <queue>
#include <iostream>
#include <limits>
#include <algorithm>


#define INF std::numeric_limits<int>::max()


struct edge {
    int to;
    int weight;
};


struct state {
    int id;
    int total_weight;
};


struct compartor {
    bool operator()(const state& l, const state& r) {
        return l.total_weight > r.total_weight;
    }
};


int main() {
    bool is_first = true;

    while (true) {
        int n, m, q, s;
        std::cin >> n >> m >> q >> s;

        if (n == 0 && m == 0 && q == 0 && s == 0) {
            break;
        }

        if (!is_first) {
            std::cout << std::endl;
        }
        is_first = false;

        std::vector<std::vector<edge>> g(n);
        for (int i = 0; i < m; i++) {
            int from, to, weight;
            std::cin >> from >> to >> weight;

            g[from].push_back({ to, weight });
        }

        std::vector<int> minimum_costs(n, INF);
        std::vector<bool> visited(n, false);

        std::priority_queue<state, std::vector<state>, compartor> pq;

        pq.push({ s, 0 });
        minimum_costs[s] = 0;

        while (!pq.empty()) {
            state node = pq.top();
            pq.pop();

            if (visited[node.id]) {
                continue;
            }

            visited[node.id] = true;
            for (const edge& e : g[node.id]) {
                if (visited[e.to]) {
                    continue;
                }

                int total_weight_to_to = node.total_weight + e.weight;
                minimum_costs[e.to] = std::min(minimum_costs[e.to], total_weight_to_to);

                pq.push({ e.to, total_weight_to_to });
            }
        }

        for (int i = 0; i < q; i++) {
            int destination;
            std::cin >> destination;

            int distance = minimum_costs[destination];
            if (distance == INF) {
                std::cout << "Impossible" << std::endl;
            }
            else {
                std::cout << distance << std::endl;
            }
        }
    }

    return 0;
}