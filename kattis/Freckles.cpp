#include <iostream>
#include <vector>
#include <cmath>
#include <queue>
#include <iomanip>


struct position {
    long double x;
    long double y;
};


struct edge {
    int v;
    long double d;
};


struct comparator {
    bool operator()(const edge& l, const edge& r) {
        return l.d > r.d;
    }
};


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    int t = 0;
    std::cin >> t;

    for (int i = 0; i < t; i++) {
        int n = 0;
        std::cin >> n;

        std::vector<position> ps(n);
        for (int j = 0; j < n; j++) {
            std::cin >> ps[j].x >> ps[j].y;
        }

        std::vector<std::vector<edge>> es(n);
        for (int j = 0; j < n; j++) {
            for (int k = j + 1; k < n; k++) {
                long double d = std::sqrt(std::pow(ps[j].x - ps[k].x, 2) + std::pow(ps[j].y - ps[k].y, 2));

                es[j].push_back({ k, d });
                es[k].push_back({ j, d });
            }
        }

        std::vector<bool> visited(n, false);
        std::priority_queue<edge, std::vector<edge>, comparator> pq;

        auto proc_func = [&visited, &es, &pq](const int id) {
            visited[id] = true;
            for (const edge& e : es[id]) {
                if (!visited[e.v]) {
                    pq.push(e);
                }
            }
        };

        proc_func(0);

        long double cost = 0;
        int edge_counter = 0;

        while (!pq.empty()) {
            edge e = pq.top();
            pq.pop();

            if (visited[e.v]) {
                continue;
            }

            proc_func(e.v);

            cost += e.d;
            edge_counter++;

            if (edge_counter == n - 1) {
                break;
            }
        }

        std::cout << std::fixed << std::setprecision(2) << cost << '\n';
    }

    return 0;
}