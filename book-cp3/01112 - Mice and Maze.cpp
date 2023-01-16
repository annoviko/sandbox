#include <algorithm>
#include <climits>
#include <vector>
#include <queue>


struct edge {
    int id;
    int weight;

    edge(const int _id, const int _weight) :
        id(_id), weight(_weight)
    { }

    bool operator<(const edge& p_edge) const {
        return weight < p_edge.weight;
    }

    bool operator>(const edge& p_edge) const {
        return weight > p_edge.weight;
    }
};


using graph = std::vector<std::vector<edge>>;


class solution {
public:
    int count_mice_exit_within_limit(const graph& g, const int p_destination, const int p_time_limit) {
        std::priority_queue<edge, std::vector<edge>, std::greater<edge>> queue;
        queue.push(edge(p_destination, 0));

        std::vector<int> cost(g.size(), INT_MAX);
        cost[p_destination] = 0;

        while (!queue.empty()) {
            edge cur = queue.top();
            queue.pop();

            if (cur.weight > cost[cur.id]) {
                continue;
            }

            for (const edge& nei : g[cur.id]) {
                const int weight_to_nei = cost[cur.id] + nei.weight;
                if (weight_to_nei < cost[nei.id]) {
                    cost[nei.id] = weight_to_nei;
                    queue.push(edge(nei.id, weight_to_nei));
                }
            }
        }

        int mice = 0;
        for (int t : cost) {
            if (t <= p_time_limit) {
                mice++;
            }
        }

        return mice;
    }
};


#include <iostream>


int main() {
    int cases;
    std::cin >> cases;

    for (int i = 0; i < cases; i++) {
        int n, e, t, m;
        std::cin >> n >> e >> t >> m;

        graph g(n);
        for (int j = 0; j < m; j++) {
            int from, to, cost;
            std::cin >> from >> to >> cost;

            g[to - 1].push_back(edge(from - 1, cost));
        }

        int time_to_exit = solution().count_mice_exit_within_limit(g, e - 1, t);
        std::cout << time_to_exit << std::endl;
        if (i != cases - 1) {
            std::cout << std::endl;
        }
    }

    return 0;
}