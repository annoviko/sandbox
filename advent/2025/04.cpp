#include <iostream>
#include <fstream>
#include <string>
#include <vector>


class solution {
private:
    std::vector<std::string> in;

public:
    solution(const std::vector<std::string>& p_in) : in(p_in) {}

public:
    int count_rolls() const {
        return find_rolls().size();
    }

    int count_removable_rolls() {
        int counter = 0;

        while (true) {
            const auto rolls_to_remove = find_rolls();
            if (rolls_to_remove.empty()) {
                break;
            }

            counter += rolls_to_remove.size();

            for (const auto& p : rolls_to_remove) {
                in[p.first][p.second] = '.';
            }
        }

        return counter;
    }

private:
    std::vector<std::pair<int, int>> find_rolls() const {
        std::vector<std::pair<int, int>> accessible_rolls;
        for (int i = 0; i < in.size(); i++) {
            for (int j = 0; j < in[i].size(); j++) {
                if (in[i][j] != '@') {
                    continue;
                }

                if (count_neighbors(i, j) < 4) {
                    accessible_rolls.push_back({ i, j });
                }
            }
        }

        return accessible_rolls;
    }

    bool is_valid_cell(int i, int j) const {
        return i >= 0 && i < in.size() && j >= 0 && j < in[i].size();
    }

    int count_cell(int i, int j) const {
        if (is_valid_cell(i, j)) {
            return (in[i][j] == '@') ? 1 : 0;
        }
        return 0;
    }

    int count_neighbors(int i, int j) const {
        const std::vector<std::pair<int, int>> neis = {
            { i - 1, j + 1 },
            { i - 1, j },
            { i - 1, j - 1 },
            { i + 1, j + 1 },
            { i + 1, j },
            { i + 1, j - 1 },
            { i, j - 1 },
            { i, j + 1 }
        };

        int counter = 0;
        for (const auto& nei : neis) {
            counter += count_cell(nei.first, nei.second);
        }
        return counter;
    }
};


std::vector<std::string> read_input() {
    std::fstream stream("input.txt");
    std::vector<std::string> input;

    for (std::string line; std::getline(stream, line);) {
        input.push_back(line);
    }

    return input;
}


int main() {
    const auto input = read_input();

    std::cout << "The accessible number of rolls: " << solution(input).count_rolls() << std::endl;
    std::cout << "The number of rolls that can be removed: " << solution(input).count_removable_rolls() << std::endl;

    return 0;
}
