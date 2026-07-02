#include <vector>
#include <queue>
#include <deque>

#if 0
    #define DIJKSTRA
#else
    #define BFS_1_0
#endif

struct entry {
    int row = -1;
    int col = -1;
    int cost = 0;
};


struct comparator {
    bool operator()(const entry& l, const entry& r) {
        return l.cost > r.cost;
    }
};


class Solution {
public:
    bool findSafeWalk(const std::vector<std::vector<int>>& grid, int health) {
#if defined(DIJKSTRA)   /* Total: O(N * M * log(N * M)) */
        std::priority_queue<entry, std::vector<entry>, comparator> pq;
        std::vector<std::vector<int>> dist(grid.size(), std::vector<int>(grid[0].size(), INT_MAX));

        pq.push({ 0, 0, grid[0][0] });
        dist[0][0] = grid[0][0];

        while (!pq.empty()) {   /* O(N * M) */
            entry cur = pq.top();   /* O(1) */
            pq.pop();               /* O(log(N * M)) */

            if (cur.cost > dist[cur.row][cur.col])
                continue;

            if (cur.row == grid.size() - 1 && cur.col == grid[0].size() - 1) {
                break;
            }

            const std::vector<std::pair<int, int>> neis = {
                { cur.row - 1, cur.col },
                { cur.row + 1, cur.col },
                { cur.row, cur.col - 1 },
                { cur.row, cur.col + 1 }
            };

            for (const auto& nei : neis) {
                if (nei.first < 0 || nei.first >= grid.size() || nei.second < 0 || nei.second >= grid[0].size()) {
                    continue;
                }

                const int candidate = cur.cost + grid[nei.first][nei.second];
                if (candidate >= health) {
                    continue;
                }

                if (candidate < dist[nei.first][nei.second]) {
                    dist[nei.first][nei.second] = candidate;
                    pq.push({ nei.first, nei.second, candidate });  /* O(log(N * M)) */
                }
            }
        }

        const int total_cost = dist.back().back();
        if (total_cost < health) {
            return true;
        }

        return false;

#elif defined(BFS_1_0)
        std::deque<entry> q;
        q.push_back({ 0, 0, grid[0][0] });

        std::vector<std::vector<int>> dist(grid.size(), std::vector<int>(grid[0].size(), INT_MAX));
        dist[0][0] = grid[0][0];

        if (dist[0][0] >= health) {
            return false;
        }

        while (!q.empty()) {    /* O(N * M) */
            const entry cur = q.front();    /* O(1) */
            q.pop_front();                  /* O(1) */

            if (cur.row == grid.size() - 1 && cur.col == grid[0].size() - 1) {
                return true;
            }

            const std::vector<std::pair<int, int>> neis = {
                { cur.row - 1, cur.col },
                { cur.row + 1, cur.col },
                { cur.row, cur.col - 1 },
                { cur.row, cur.col + 1 }
            };

            for (const auto& nei : neis) {
                if (nei.first < 0 || nei.first >= grid.size() || nei.second < 0 || nei.second >= grid[0].size()) {
                    continue;
                }

                const int candidate = cur.cost + grid[nei.first][nei.second];
                if (candidate >= dist[nei.first][nei.second]) {
                    continue;
                }

                dist[nei.first][nei.second] = candidate;

                if (grid[nei.first][nei.second] == 0) {
                    q.push_front({ nei.first, nei.second, cur.cost });      /* O(1) amortized */
                }
                else if (cur.cost + 1 < health) {
                    q.push_back({ nei.first, nei.second, cur.cost + 1 });   /* O(1) */
                }
            }
        }

        return false;
#endif
    }
};