#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>


struct position {
    int row = 0;
    int col = 0;
};


class solution {
public:
    int solve(const std::vector<std::vector<int>>& map) {
        std::vector<std::vector<bool>> visited(map.size(), std::vector<bool>(map[0].size(), false));

        int suitable_area = 0;

        for (int i = 0; i < map.size(); i++) {
            for (int j = 0; j < map[i].size(); j++) {
                if (visited[i][j]) {
                    continue;
                }

                suitable_area += bfs_cluster(i, j, map, visited);
            }
        }

        return suitable_area;
    }

private:
    int bfs_cluster(int row, int col, const std::vector<std::vector<int>>& map, std::vector<std::vector<bool>>& visited) {
        int id = map[row][col]; /* height */
        
        std::queue<position> to_process;
        to_process.push({ row, col });

        visited[row][col] = true;

        int area = 1;
        bool suitable = true;

        while (!to_process.empty()) {
            auto cur_pos = to_process.front();
            to_process.pop();
            
            auto neis = get_neighbors(cur_pos.row, cur_pos.col, visited.size(), visited[0].size());
            for (auto& nei_pos : neis) {
                if (map[nei_pos.row][nei_pos.col] < id) {
                    suitable = false;   /* but continue in order to mark them all visited */
                }

                if (visited[nei_pos.row][nei_pos.col]) {
                    continue;
                }

                if (map[nei_pos.row][nei_pos.col] == id) {
                    visited[nei_pos.row][nei_pos.col] = true;
                    to_process.push(nei_pos);
                    area++;
                }
            }
        }

        return suitable ? area : 0;
    }

    std::vector<position> get_neighbors(int row, int col, int max_rows, int max_cols) {
        std::vector<position> result;

        if (row - 1 >= 0) {
            result.push_back({ row - 1, col });
        }

        if (col - 1 >= 0) {
            result.push_back({ row, col - 1 });
        }

        if (row + 1 < max_rows) {
            result.push_back({ row + 1, col });
        }

        if (col + 1 < max_cols) {
            result.push_back({ row, col + 1 });
        }

        return result;
    }
};


int main() {
    int cols, rows;
    std::cin >> cols >> rows;

    std::vector<std::vector<int>> map;
    for (int i = 0; i < rows; i++) {
        std::vector<int> row;

        for (int j = 0; j < cols; j++) {
            int height = 0;
            std::cin >> height;

            row.push_back(height);
        }

        map.push_back(row);
    }

    std::cout << solution().solve(map) << std::endl;
    return 0;
}