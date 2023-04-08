struct position {
    int row = 0;
    int col = 0;
};


class Solution {
public:
    int maxAreaOfIsland(const std::vector<std::vector<int>>& grid) {
        std::vector<std::vector<bool>> visited(grid.size(), std::vector<bool>(grid[0].size(), false));

        int max_area = 0;
        for (int row = 0; row < grid.size(); row++) {
            for (int col = 0; col < grid[0].size(); col++) {
                if (grid[row][col] == 0) {
                    continue;
                }

                if (visited[row][col]) {
                    continue;
                }

                int area = dfs(row, col, grid, visited);
                // int area = bfs(row, col, grid, visited);
                max_area = std::max(area, max_area);
            }
        }

        return max_area;
    }


    int dfs(const int row, const int col, const std::vector<std::vector<int>>& grid, std::vector<std::vector<bool>>& visited) {
        if ((row >= 0) && (col >= 0) && (row < grid.size()) && (col < grid[0].size()) && (grid[row][col] == 1) && (visited[row][col] != true)) {
            visited[row][col] = true;
            
            return 1 + dfs(row - 1, col, grid, visited) + dfs(row + 1, col, grid, visited) + dfs(row, col - 1, grid, visited) + dfs(row, col + 1, grid, visited);
        }

        return 0;
    }


    std::vector<position> get_neighbors(const int row, const int col, const std::vector<std::vector<int>>& grid) {
        std::vector<position> neis;

        if (row - 1 >= 0) {
            neis.push_back({ row - 1, col });
        }

        if (col - 1 >= 0) {
            neis.push_back({ row, col - 1 });
        }

        if (row + 1 < grid.size()) {
            neis.push_back({ row + 1, col });
        }

        if (col + 1 < grid[0].size()) {
            neis.push_back({ row, col + 1 });
        }

        return neis;
    }


    int bfs(const int row, const int col, const std::vector<std::vector<int>>& grid, std::vector<std::vector<bool>>& visited) {
        std::queue<position> to_process;
        to_process.push({ row, col });

        int area = 0;
        visited[row][col] = true;

        while(!to_process.empty()) {
            auto cur = to_process.front();
            to_process.pop();

            area++;

            auto neis = get_neighbors(cur.row, cur.col, grid);
            for (auto & nei : neis) {
                if (visited[nei.row][nei.col] || (grid[nei.row][nei.col] == 0)) {
                    continue;
                }
                
                visited[nei.row][nei.col] = true;
                to_process.push(nei);
            }
        }

        return area;
    }
};