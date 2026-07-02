#include <vector>
#include <queue>


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
        std::priority_queue<entry, std::vector<entry>, comparator> pq;
        std::vector<std::vector<int>> dist(grid.size(), std::vector<int>(grid[0].size(), INT_MAX));
        std::vector<std::vector<int>> visited(grid.size(), std::vector<int>(grid[0].size(), false));

        pq.push({ 0, 0, grid[0][0] });
        dist[0][0] = grid[0][0];

        while (!pq.empty()) {
            entry cur = pq.top();
            pq.pop();

            if (visited[cur.row][cur.col]) {
                continue;
            }

            if (cur.cost > dist[cur.row][cur.col])
                continue;

            if (cur.row == grid.size() - 1 && cur.col == grid[0].size() - 1) {
                break;
            }

            visited[cur.row][cur.col] = true;

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

                if (visited[nei.first][nei.second]) {
                    continue;
                }

                const int candidate = cur.cost + grid[nei.first][nei.second];
                if (candidate >= health) {
                    continue;
                }

                if (candidate < dist[nei.first][nei.second]) {
                    dist[nei.first][nei.second] = candidate;
                    pq.push({ nei.first, nei.second, candidate });
                }
            }
        }

        const int total_cost = dist.back().back();
        if (total_cost < health) {
            return true;
        }

        return false;
    }
};