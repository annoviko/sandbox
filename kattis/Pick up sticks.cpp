#include <iostream>
#include <vector>
#include <queue>


using graph = std::vector<std::vector<int>>;


class solution {
public:
    std::vector<int> solve(const graph& g) {
        std::vector<int> inc_conn(g.size(), 0);
        for (int from = 0; from < g.size(); from++) {
            for (int to : g[from]) {
                inc_conn[to]++;
            }
        }

        std::queue<int> to_proc;
        for (int id = 0; id < g.size(); id++) {
            if (inc_conn[id] == 0) {
                to_proc.push(id);
            }
        }

        std::vector<int> order;
        while (!to_proc.empty()) {
            int cur_id = to_proc.front();
            to_proc.pop();

            order.push_back(cur_id);

            for (int nei_id : g[cur_id]) {
                inc_conn[nei_id]--;
                if (inc_conn[nei_id] == 0) {
                    to_proc.push(nei_id);
                }
            }
        }

        if (order.size() != g.size()) {
            return { };
        }
        return order;
    }
};


int main() {
    int nodes, edges;
    std::cin >> nodes >> edges;

    graph g(nodes);
    for (int i = 0; i < edges; i++) {
        int from, to;
        std::cin >> from >> to;

        g[from - 1].push_back(to - 1);
    }

    auto order = solution().solve(g);
    if (order.empty()) {
        std::cout << "IMPOSSIBLE" << std::endl;
    }
    else {
        for (int id : order) {
            std::cout << id + 1 << std::endl;
        }
    }

    return 0;
}
