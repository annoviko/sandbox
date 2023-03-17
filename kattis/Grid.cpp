#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <queue>


struct position {
    int row = 0;
    int col = 0;
};


class solution {
public:
    std::int64_t get_steps(const std::vector<std::string>& field) {
        std::vector<std::vector<int>> steps(field.size(), std::vector<int>(field[0].size(), -1));

        std::queue<position> queue;
        queue.push({ 0, 0 });
        steps[0][0] = 0;

        while (!queue.empty()) {
            auto cur_pos = queue.front();
            queue.pop();

            auto neighbors = get_neigbors(field, cur_pos);
            for (const auto& nei : neighbors) {
                if (steps[nei.row][nei.col] != -1) {
                    continue;
                }

                steps[nei.row][nei.col] = steps[cur_pos.row][cur_pos.col] + 1;
                queue.push(nei);

                if ((nei.row == field.size() - 1) && (nei.col == field[0].size() - 1)) {
                    return steps[nei.row][nei.col];
                }
            }
        }

        return steps[field.size() - 1][field[0].size() - 1];
    }

private:
    std::vector<position> get_neigbors(const std::vector<std::string>& field, const position& p_pos) {
        std::vector<position> result;

        const int value = field[p_pos.row][p_pos.col] - '0';

        if (p_pos.row + value < field.size()) {
            result.push_back({ p_pos.row + value, p_pos.col } );
        }
        
        if (p_pos.row - value >= 0) {
            result.push_back({ p_pos.row - value, p_pos.col });
        }

        if (p_pos.col + value < field[0].size()) {
            result.push_back({ p_pos.row, p_pos.col + value });
        }

        if (p_pos.col - value >= 0) {
            result.push_back({ p_pos.row, p_pos.col - value });
        }

        return result;
    }
};


int main() {
    int n, m;
    std::cin >> n >> m;

    std::vector<std::string> field;
    for (int i = 0; i < n; i++) {
        std::string line;
        std::cin >> line;

        field.push_back(line);
    }

    std::cout << solution().get_steps(field) << std::endl;

    return 0;
}