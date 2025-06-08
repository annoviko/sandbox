#include <iostream>
#include <vector>
#include <queue>
#include <limits>


struct edge {
    int to;
    int distance;
};


struct state {
    int distance = std::numeric_limits<int>::max();
    int accumulator = 0;
    std::vector<int> prev_nodes;
};


struct node {
    int id;
    int distance;
};


struct node_comparator {
    bool operator()(const node& l, const node& r) {
        return l.distance > r.distance;
    }
};


int main() {
    int n, m;
    std::cin >> n >> m;

    std::vector<std::vector<edge>> g(n);
    for (int i = 0; i < m; i++) {
        int from, to, distance;
        std::cin >> from >> to >> distance;

        g[from].push_back({ to, distance });
        g[to].push_back({ from, distance });
    }

    std::vector<state> dist(n);
    std::vector<bool> visited(n, false);

    std::priority_queue<node, std::vector<node>, node_comparator> pq;

    pq.push({ 0, {} });
    dist[0].distance = 0;

    while (!pq.empty()) {
        node cur = pq.top();
        pq.pop();

        if (visited[cur.id]) {
            continue;
        }

        visited[cur.id] = true;

        for (const edge& e : g[cur.id]) {
            if (visited[e.to]) {
                continue;
            }

            int distance_to_nei = e.distance + cur.distance;
            if (distance_to_nei < dist[e.to].distance) {
                dist[e.to] = { distance_to_nei, e.distance, { cur.id } };
            }
            else if (distance_to_nei == dist[e.to].distance) {
                dist[e.to].prev_nodes.push_back(cur.id);
                dist[e.to].accumulator += e.distance;
            }
            else {
                continue;
            }

            pq.push({ e.to, distance_to_nei });
        }
    }

    std::queue<int> ids;
    ids.push(n - 1);

    std::vector<bool> back_visited(n, false);
    back_visited[n - 1] = true;

    int total_distance = 0;
    while (!ids.empty()) {
        int cur = ids.front();
        ids.pop();

        total_distance += dist[cur].accumulator;
        for (int parent : dist[cur].prev_nodes) {
            if (back_visited[parent]) {
                continue;
            }

            back_visited[parent] = true;
            ids.push(parent);
        }
    }

    std::cout << total_distance * 2 << std::endl;
    return 0;
}