// https://open.kattis.com/problems/lostinthewoods

#include <cstdint>
#include <iostream>
#include <iomanip>
#include <queue>
#include <vector>
#include <unordered_map>


using graph = std::vector<std::vector<int>>;


struct context {
    int idx = 0;
    long double probability = 0.0;
};


class solution {
public:
    double get_avg_escape_time(const graph& p_graph) {
        std::queue<context> to_visit;
        to_visit.push({ 0, 1.0 });

        long double avg_time = 0.0;
        long double cur_time = 0.0;

        while (!to_visit.empty()) {
            cur_time++;

            std::unordered_map<int, context> next;
            long double remaining_time = 0.0;

            while (!to_visit.empty()) {
                context cur = to_visit.front();
                to_visit.pop();

                const long double probability = cur.probability / (long double) p_graph[cur.idx].size();

                for (int nei_idx : p_graph[cur.idx]) {
                    if (nei_idx == p_graph.size() - 1) {
                        avg_time += cur_time * probability;
                        continue;
                    }

                    remaining_time += cur_time * probability;

                    if (next.find(nei_idx) == next.end()) {
                        next[nei_idx].idx = nei_idx;
                    }

                    next[nei_idx].probability += probability;
                }
            }

            if (remaining_time < (long double) 0.000000001) {
                continue;
            }

            for (auto& pair : next) {
                to_visit.push(pair.second);
            }
        }

        return avg_time;
    }
};


int main() {
    int n, m;
    std::cin >> n >> m;

    graph g = graph(n);

    for (int i = 0; i < m; i++) {
        int from, to;
        std::cin >> from >> to;

        g[from].push_back(to);
        g[to].push_back(from);
    }

    std::cout << std::setprecision(10) << solution().get_avg_escape_time(g) << std::endl;

    return 0;
}