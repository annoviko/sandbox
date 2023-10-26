#include <climits>
#include <iostream>
#include <vector>
#include <queue>


struct edge {
    int idx;
    int fuel;
    int weight; /* for graph distance, for queue - price */

public:
    edge(const int i, const int f, const int w) :
        idx(i), fuel(f), weight(w)
    { }

public:
    bool operator<(const edge& p_other) const {
        return weight < p_other.weight;
    }

    bool operator>(const edge& p_other) const {
        return weight > p_other.weight;
    }
};


using graph = std::vector<std::vector<edge>>;


class solution {
public:
    int find_cheapest_way(const graph p_graph, const std::vector<int>& p_prices, const int p_capacity, const int from, const int to) {
        std::priority_queue <edge, std::vector<edge>, std::greater<edge>> queue;
        queue.push(edge(from, 0, 0));

        std::vector<std::vector<int>> cost(p_graph.size(), std::vector<int>(p_capacity + 1, INT_MAX));
        cost[from][0] = 0;

        while (!queue.empty()) {
            auto node = queue.top();
            queue.pop();

            if (node.idx == to) {
                return node.weight;
            }

            if (node.weight > cost[node.idx][node.fuel]) {
                continue;
            }

            /* add node where we get additional gallon in this city */
            if (node.fuel < p_capacity && node.weight + p_prices[node.idx] < cost[node.idx][node.fuel + 1]) {
                const int price = node.weight + p_prices[node.idx];

                cost[node.idx][node.fuel + 1] = price;
                queue.push(edge(node.idx, node.fuel + 1, price));
            }

            for (const auto& nei : p_graph[node.idx]) {
                if (node.fuel < nei.weight) { /* we do not have enough fuel to reach the city */
                    continue;
                }

                const int remaining_fuel = node.fuel - nei.weight;
                if (node.weight < cost[nei.idx][remaining_fuel]) {
                    cost[nei.idx][remaining_fuel] = node.weight;
                    queue.push(edge(nei.idx, remaining_fuel, node.weight)); /* add node with remaining fuel */
                }
            }
        }

        return INT_MAX;
    }
};



int main() {
    int n, m;
    std::cin >> n >> m;

    std::vector<int> prices(n, 0);
    for (int i = 0; i < n; i++) {
        int cost;
        std::cin >> cost;

        prices[i] = cost;
    }

    graph g(n);
    for (int i = 0; i < m; i++) {
        int from, to, distance;
        std::cin >> from >> to >> distance;

        g[from].push_back(edge(to, 0, distance));
        g[to].push_back(edge(from, 0, distance));
    }

    int k;
    std::cin >> k;

    for (int i = 0; i < k; i++) {
        int capacity, from, to;
        std::cin >> capacity >> from >> to;

        int cost = solution().find_cheapest_way(g, prices, capacity, from, to);

        if (cost == INT_MAX) {
            std::cout << "impossible" << std::endl;
        }
        else {
            std::cout << cost << std::endl;
        }
    }

    return 0;
}