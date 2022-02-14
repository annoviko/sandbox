#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>


using height_row = std::vector<std::int32_t>;
using height_field = std::vector<height_row>;


height_field read_input() {
    std::ifstream stream("input.txt");

    height_field result;

    for (std::string str_row; std::getline(stream, str_row);) {
        height_row row(str_row.size(), 0);
        for (std::size_t i = 0; i < str_row.size(); i++) {
            row[i] = str_row[i] - '0';
        }

        result.push_back(row);
    }

    return result;
}


struct position {
    std::int32_t x = 0;
    std::int32_t y = 0;
};


class field_analyser {
private:
    using visit_row = std::vector<bool>;
    using visit_field = std::vector<visit_row>;

private:
    height_field m_field;

public:
    field_analyser(const height_field& p_field) :
        m_field(p_field)
    { }

public:
    std::int64_t get_risk_level() {
        const auto lowest_heights = get_lowest_hights();

        std::int64_t result = 0;
        for (const auto& point : lowest_heights) {
            result += m_field[point.y][point.x] + 1;
        }

        return result;
    }

    std::size_t get_three_highest_basins() {
        const auto lowest_heights = get_lowest_hights();

        std::vector<std::size_t> sizes = { };
        for (const position& point : lowest_heights) {
            const std::size_t square = traverse(point);
            sizes.push_back(square);
        }

        std::sort(sizes.begin(), sizes.end());

        std::size_t result = 1;
        std::size_t amount = 0;
        for (auto iter = sizes.rbegin(); iter != sizes.rend(); iter++) {
            result *= *iter;

            if (++amount >= 3) {
                break;
            }
        }

        return result;
    }

private:
    std::size_t traverse(const position& p_position) {
        std::size_t result = 1;

        visit_field visited(m_field.size(), visit_row(m_field[0].size(), false));

        std::list<position> to_process = { p_position };
        while(!to_process.empty()) {
            const position& cur = to_process.front();
            visited[cur.y][cur.x] = true;

            const auto cur_value = m_field[cur.y][cur.x];

            const auto neighbors = get_neighbors(cur.y, cur.x);
            for (const auto point : neighbors) {
                const auto value_neighbor = m_field[point.y][point.x];
                if ((value_neighbor != 9) && (!visited[point.y][point.x])) {
                    to_process.push_back({ point.x, point.y });
                    result++;
                }

                visited[point.y][point.x] = true;
            }

            to_process.pop_front();
        }

        return result;
    }

    std::vector<position> get_lowest_hights() {
        std::vector<position> result;

        for (std::int32_t i = 0; i < static_cast<std::int32_t>(m_field.size()); i++) {
            for (std::int32_t j = 0; j < static_cast<std::int32_t>(m_field[0].size()); j++) {
                if (is_lowest(i, j)) {
                    result.push_back({ j, i });
                }
            }
        }

        return result;
    }

    bool is_lowest(const std::int32_t p_row, const std::int32_t p_col) const {
        const auto neigbors = get_neighbors(p_row, p_col);
        const auto value = m_field[p_row][p_col];

        for (const auto point : neigbors) {
            if (m_field[point.y][point.x] <= value) {
                return false;
            }
        }

        return true;
    }

    std::vector<position> get_neighbors(const std::int32_t p_row, const std::int32_t p_col) const {
        std::vector<position> neighbors = { };

        if (p_col - 1 >= 0) {
            neighbors.push_back({ p_col - 1, p_row });
        }

        if (p_row - 1 >= 0) {
            neighbors.push_back({ p_col, p_row - 1 });
        }

        if (p_col + 1 < m_field[0].size()) {
            neighbors.push_back({ p_col + 1, p_row });
        }

        if (p_row + 1 < m_field.size()) {
            neighbors.push_back({ p_col, p_row + 1 });
        }

        return neighbors;
    }
};


int main() {
    const auto field = read_input();

    std::cout << "Risk level: " << field_analyser(field).get_risk_level() << std::endl;
    std::cout << "Three highest basins: " << field_analyser(field).get_three_highest_basins() << std::endl;

    return 0;
}