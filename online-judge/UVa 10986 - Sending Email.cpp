#include <climits>
#include <queue>
#include <vector>


struct edge {
    int id = -1;
    int weight = -1;

    edge(const int p_node, const int p_weight) :
        id(p_node), weight(p_weight)
    { }

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
    int find_shortest_time(const graph& g, const int p_source, const int p_destination) {
        if (p_source == p_destination) {
            return 0;
        }

        std::priority_queue<edge, std::vector<edge>, std::greater<edge>> queue;
        queue.push(edge(p_source, 0));

        std::vector<int> cost(g.size(), INT_MAX);
        cost[p_source] = 0;

        while (!queue.empty()) {
            edge cur = queue.top();
            queue.pop();

            if (cost[cur.id] < cur.weight) {
                continue;
            }

            for (const edge& nei : g[cur.id]) {
                int weight = cost[cur.id] + nei.weight;
                if (cost[nei.id] > weight) {
                    cost[nei.id] = weight;
                    queue.push(nei);
                }
            }
        }

        return cost[p_destination];
    }
};


#include <iostream>


int main() {
    int cases;
    std::cin >> cases;

    for (int i = 0; i < cases; i++) {
        int n, m, source, destination;
        std::cin >> n >> m >> source >> destination;

        graph g(n);
        for (int j = 0; j < m; j++) {
            int from, to, cost;
            std::cin >> from >> to >> cost;

            g[from].push_back(edge(to, cost));
            g[to].push_back(edge(from, cost));
        }

        int time = solution().find_shortest_time(g, source, destination);
        if (time == INT_MAX) {
            std::cout << "Case #" << i + 1 << ": unreachable" << std::endl;
        }
        else {
            std::cout << "Case #" << i + 1 << ": " << time << std::endl;
        }
    }

    return 0;
}