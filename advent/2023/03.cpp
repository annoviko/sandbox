#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>


using engine_scheme = std::vector<std::string>;


struct position {
    int row = 0;
    int col = 0;
};


class solution {
    engine_scheme m_scheme;

public:
    solution(const engine_scheme& scheme) :
        m_scheme(scheme)
    { }


public:
    int count_sum_part_numbers() const {
        int sum = 0;

        for (int r = 0; r < m_scheme.size(); r++) {
            std::string number;
            bool found_part_number = false;

            for (int c = 0; c < m_scheme[r].size(); c++) {
                const char symbol = m_scheme[r][c];

                if (std::isdigit(symbol)) {
                    number.push_back(symbol);
                    found_part_number |= is_part_number(r, c);
                }
                else if (!number.empty()) {
                    if (found_part_number) {
                        sum += std::stoi(number);
                    }

                    found_part_number = false;
                    number.clear();
                }
            }

            if (found_part_number && !number.empty()) {
                sum += std::stoi(number);
            }
        }

        return sum;
    }


    std::int64_t count_gear_ratio() const {
        std::vector<std::vector<std::int64_t>> number_map_ids(m_scheme.size(), std::vector<std::int64_t>(m_scheme[0].size(), -1));
        std::vector<int> ids_to_number;

        std::int64_t current_id = 0;
        std::vector<position> gears;

        for (int r = 0; r < m_scheme.size(); r++) {
            std::string number;

            for (int c = 0; c < m_scheme[r].size(); c++) {
                const char symbol = m_scheme[r][c];
                if (std::isdigit(symbol)) {
                    number.push_back(symbol);
                    number_map_ids[r][c] = current_id;
                }
                else {
                    if (!number.empty()) {
                        ids_to_number.push_back(std::stoi(number));

                        number.clear();
                        current_id = ids_to_number.size();
                    }

                    if (symbol == '*') {
                        gears.push_back({ r, c });
                    }
                }
            }

            if (!number.empty()) {
                ids_to_number.push_back(std::stoi(number));
                current_id = ids_to_number.size();
            }
        }

        std::int64_t sum = 0;
        for (auto& pos : gears) {
            auto neighbors = get_neighbors(pos.row, pos.col);
            std::unordered_set<std::int64_t> unique_id_numbers;

            for (auto& nei : neighbors) {
                const std::int64_t id_nei = number_map_ids[nei.row][nei.col];

                if (id_nei != -1) {
                    unique_id_numbers.insert(id_nei);
                }
            }

            if (unique_id_numbers.size() == 2) {
                std::int64_t product = 1;
                for (auto id : unique_id_numbers) {
                    product *= ids_to_number[id];
                }

                sum += product;
            }
        }

        return sum;
    }

private:
    bool is_inside(const int p, const int l, const int r) const {
        return (p >= l && p < r);
    }


    std::vector<position> get_neighbors(const int row, const int col) const {
        std::vector<position> result;

        for (int r = row - 1; r <= row + 1; r++) {
            for (int c = col - 1; c <= col + 1; c++) {
                if ((r == row) && (c == col)) {
                    continue;
                }

                if (is_inside(r, 0, m_scheme.size()) && is_inside(c, 0, m_scheme[0].size())) {
                    result.push_back({ r, c });
                }
            }
        }

        return result;
    }


    bool is_part(const int row, const int col) const {
        return !std::isdigit(m_scheme[row][col]) && m_scheme[row][col] != '.';
    }


    bool is_part_number(const int row, const int col) const {
        auto neighbors = get_neighbors(row, col);
        for (auto nei : neighbors) {
            if (is_part(nei.row, nei.col)) {
                return true;
            }
        }

        return false;
    }
};


engine_scheme read_input() {
    std::ifstream stream("input.txt");

    engine_scheme scheme;
    for (std::string line; std::getline(stream, line);) {
        scheme.push_back(line);
    }

    return scheme;
}


int main() {
    auto input = read_input();

    std::cout << "The sum of all of the part numbers in the engine schematic: " << solution(input).count_sum_part_numbers() << std::endl;
    std::cout << "The sum of all of the gear ratios in the engine schematic: " << solution(input).count_gear_ratio() << std::endl;

    return 0;
}
