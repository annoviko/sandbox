#include <climits>
#include <iostream>
#include <list>
#include <set>
#include <queue>
#include <vector>
#include <unordered_map>
#include <unordered_set>


struct to_edge {
    int id = -1;
    std::int64_t weight = -1;
};


bool operator<(const to_edge& l, const to_edge& r) {
    return l.weight < r.weight || (!(r.weight < l.weight) && l.id < r.id);
}


struct edge {
    int from = -1;
    int to = -1;
    std::int64_t weight = -1;
};


using graph = std::vector<std::vector<to_edge>>;


class solution {
public:
    std::list<int> get_route(const graph& g, const std::vector<edge>& e) {
        /* Dijkstra to find the shortest pathes from Amsterdam and save optimal edges in `parents` */
        std::vector<std::int64_t> weights(g.size(), INT_MAX);
        std::vector<int> parents(g.size(), INT_MAX);

        std::set<to_edge> queue;
        queue.insert({ 1, 0 });
        weights[1] = 0;

        while (!queue.empty()) {
            auto cur = *(queue.begin());
            queue.erase(queue.begin());

            for (const to_edge& nei : g[cur.id]) {
                std::int64_t weight_to_nei = nei.weight + weights[cur.id];
                if (weight_to_nei < weights[nei.id]) {
                    queue.erase({ nei.id, weight_to_nei  });
                    queue.insert({ nei.id, weight_to_nei });

                    weights[nei.id] = weight_to_nei;
                    parents[nei.id] = cur.id;
                }
            }
        }

        /* Create new graph without optimal edges that are in `parents` */
        std::vector<std::vector<int>> g_cutted(g.size());

        for (int cur = 0; cur < g.size(); cur++) {
            for (const to_edge& nei : g[cur]) {
                if (parents[cur] == nei.id) {
                    continue;
                }

                g_cutted[cur].push_back(nei.id);
            }
        }


        /* BFS to get none optimal path to Amsterdam from Delft */
        std::queue<int> bfs_queue;
        std::vector<bool> visited(g_cutted.size(), false);
        std::vector<int> bfs_parents(g_cutted.size(), INT_MAX);

        bfs_queue.push(0);
        visited[0] = true;

        while (!bfs_queue.empty()) {
            int cur_id = bfs_queue.front();
            bfs_queue.pop();

            for (int nei : g_cutted[cur_id]) {
                if (visited[nei]) {
                    continue;
                }

                visited[nei] = true;
                bfs_parents[nei] = cur_id;
                bfs_queue.push(nei);
            }
        }

        if (bfs_parents[1] == INT_MAX) {
            return { };
        }

        /* Return the path proper order */
        std::list<int> none_optimal_path;
        for (int cur_id = 1; cur_id != INT_MAX; cur_id = bfs_parents[cur_id]) {
            none_optimal_path.push_front(cur_id);
        }

        return none_optimal_path;
    }
};


int main() {
    int n, m;
    std::cin >> n >> m;

    std::vector<edge> e;
    graph g(n);
    for (int i = 0; i < m; i++) {
        int from, to, cost;
        std::cin >> from >> to >> cost;

        e.push_back({ from, to, cost });

        g[from].push_back({ to, cost });
        g[to].push_back({ from, cost });
    }

    auto path = solution().get_route(g, e);
    if (path.empty()) {
        std::cout << "impossible" << std::endl;
    }
    else {
        std::cout << path.size() << " ";
        for (auto id : path) {
            std::cout << id << " ";
        }
        std::cout << std::endl;
    }
}
