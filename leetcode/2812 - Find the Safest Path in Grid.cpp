#include <iostream>
#include <vector>
#include <queue>


struct entry {
    int r = -1;
    int c = -1;
    int cost = 0;
};


struct comparator {
    bool operator()(const entry& l, const entry& r) const {
        return l.cost < r.cost;
    }
};


class Solution {
public:
    int maximumSafenessFactor(const std::vector<std::vector<int>>& grid) {
        std::vector<std::vector<int>> heat_map(grid.size(), std::vector<int>(grid[0].size(), -1));
        std::queue<entry> q;

        for (int i = 0; i < grid.size(); i++) {
            for (int j = 0; j < grid[0].size(); j++) {
                if (grid[i][j] == 1) {
                    q.push({ i, j, 0 });
                    heat_map[i][j] = 0;
                }
            }
        }

        const std::vector<std::pair<int, int>> dirs = {
            { -1,  0 },
            {  1,  0 },
            {  0, -1 },
            {  0,  1 }
        };

        while (!q.empty()) {
            const auto cur = q.front();
            q.pop();

            for (const auto& dir : dirs) {
                const int row = cur.r + dir.first;
                const int col = cur.c + dir.second;

                if (row < 0 || row >= grid.size() || col < 0 || col >= grid[0].size()) {
                    continue;
                }

                if (heat_map[row][col] != -1) {
                    continue;
                }

                heat_map[row][col] = cur.cost + 1;
                q.push({ row, col, cur.cost + 1 });
            }
        }

        std::priority_queue<entry, std::vector<entry>, comparator> pq;
        pq.push({ 0, 0, heat_map[0][0] });

        std::vector<std::vector<int>> safety(grid.size(), std::vector<int>(grid[0].size(), -1));
        safety[0][0] = heat_map[0][0];

        while (!pq.empty()) {
            const auto cur = pq.top();
            pq.pop();

            if ((cur.r == grid.size() - 1) && (cur.c == grid[0].size() - 1)) {
                break;
            }

            if (cur.cost < safety[cur.r][cur.c]) { /* the path is less safe */
                continue;
            }

            for (const auto& dir : dirs) {
                const int row = cur.r + dir.first;
                const int col = cur.c + dir.second;

                if (row < 0 || row >= grid.size() || col < 0 || col >= grid[0].size()) {
                    continue;
                }

                const int candidate = std::min(cur.cost, heat_map[row][col]);
                if (candidate > safety[row][col]) {    /* path is way more safe */
                    pq.push({ row, col, candidate });
                    safety[row][col] = candidate;
                }
            }
        }

        return safety.back().back();
    }
};