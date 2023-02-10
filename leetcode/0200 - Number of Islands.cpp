struct position {
    int i = 0;
    int j = 0;
};


class Solution {
public:
    int numIslands(std::vector<std::vector<char>>& grid) {
        int result = 0;

        for (int i = 0; i < grid.size(); i++) {
            for (int j = 0; j < grid[i].size(); j++) {
                if (grid[i][j] == '1') {
                    bfs(i, j, grid);
                    result++;
                }
            }
        }

        return result;
    }

private:
    std::vector<position> get_neigbours(int i, int j, std::vector<std::vector<char>>& p_grid) {
        std::vector<position> result;

        if (i - 1 >= 0) {
            result.push_back({ i - 1, j });
        }

        if (i + 1 < p_grid.size()) {
            result.push_back({ i + 1, j });
        }

        if (j - 1 >= 0) {
            result.push_back({ i, j - 1 });
        }

        if (j + 1 < p_grid[i].size()) {
            result.push_back({ i, j + 1 });
        }

        return result;
    }


    void bfs(int i, int j, std::vector<std::vector<char>>& p_grid) {
        std::queue<position> to_process;
        to_process.push({ i, j });

        p_grid[i][j] = '0';

        while(!to_process.empty()) {
            position cur = to_process.front();
            to_process.pop();

            std::vector<position> neighbors = get_neigbours(cur.i, cur.j, p_grid);
            for (position & nei : neighbors) {
                if (p_grid[nei.i][nei.j] == '1') {
                    to_process.push(nei);
                }

                p_grid[nei.i][nei.j] = '0';
            }
        }
    }
};