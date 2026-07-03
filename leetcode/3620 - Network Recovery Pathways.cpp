#include <iostream>
#include <vector>
#include <queue>


struct entry {
    int id;
    long long cost;
};


struct comparator {
    bool operator()(const entry& l, const entry& r) const {
        return l.cost > r.cost;
    }
};


class Solution {
public:
    int findMaxPathScore(const std::vector<std::vector<int>>& edges, const std::vector<bool>& online, long long k) {
        int left = INT_MAX;
        int right = 0;

        std::vector<std::vector<std::pair<int, int>>> g(online.size());
        for (const auto& e : edges) {
            const int from = e[0];
            const int to = e[1];
            const int w = e[2];

            if (!online[from] || !online[to]) {
                continue;   /* no need to have this edge in graph - one of the nodes is offline */
            }

            if (w > k) {
                continue;   /* no need to consider edges which are already larger than maximum cost */
            }

            g[from].push_back({ to, w });
            left = std::min(left, w);
            right = std::max(right, w);
        }

        if (!dijkstra(g, k, left)) {
            return -1;  /* there is no such path */
        }

        /* find the biggest minimum edge where path is smaller than k */
        while (left <= right) {     /* O(log(N)) */
            int mid = (right + left) / 2;
            if (!dijkstra(g, k, mid)) {     /* O(N^2) */
                right = mid - 1;
            }
            else {
                left = mid + 1;
            }
        }

        return right;   /* O(N^2 * log(N)) */
    }

private:
    bool dijkstra(const std::vector<std::vector<std::pair<int, int>>>& g, long long k, int wlim) {
        std::vector<int> dist(g.size(), INT_MAX);
        std::priority_queue<entry, std::vector<entry>, comparator> pq;
        pq.push({ 0, 0 });

        while (!pq.empty()) {
            auto cur = pq.top();
            pq.pop();

            if (cur.cost > k) {
                return false;
            }

            if (cur.id == g.size() - 1) {
                return true;
            }

            if (cur.cost > dist[cur.id]) {
                continue;
            }

            for (const auto& e : g[cur.id]) {
                if (e.second < wlim) {
                    continue;   /* we need the shortest path with minimum edge >= `wlim` */
                }

                int candidate = e.second + cur.cost;
                if (candidate < dist[e.first]) {
                    pq.push({ e.first, candidate });
                    dist[e.first] = candidate;
                }
            }
        }

        return false;
    }
};