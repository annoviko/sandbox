#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>


struct position {
    int row;
    int col;
};


class solution {
private:
    std::vector<std::string> m_map;
    std::unordered_map<int, std::unordered_set<int>> m_pos;

public:
    solution(const std::vector<std::string>& p_map) :
        m_map(p_map)
    { }

public:
    std::uint64_t solve(const int steps, const int row_begin, const int col_begin) {
        m_pos.clear();

        m_pos[row_begin].insert(col_begin);

        for (int i = 0; i < steps; i++) {
            std::unordered_map<int, std::unordered_set<int>> next_spots;

            for (const auto& row_iter : m_pos) {
                int row = row_iter.first;
                for (int col : row_iter.second) {
                    auto garden_spots = get_garder_spots({ row, col });

                    for (const auto& spot : garden_spots) {
                        next_spots[spot.row].insert(spot.col);
                    }
                }
            }

            m_pos = std::move(next_spots);
        }

        std::uint64_t counter = 0;
        for (const auto& row_iter : m_pos) {
            counter += row_iter.second.size();
        }

        return counter;
    };


private:
    std::vector<position> get_garder_spots(const position& p_pos) const {
        std::vector<position> candidates = {
            { p_pos.row - 1, p_pos.col }, { p_pos.row + 1, p_pos.col },
            { p_pos.row, p_pos.col - 1 }, { p_pos.row, p_pos.col + 1 }
        };

        std::vector<position> garden_spots;
        for (const auto& p : candidates) {
            if (is_gardern_spot(p)) {
                garden_spots.push_back(p);
            }
        }

        return garden_spots;
    }

    bool is_gardern_spot(const position& p_pos) const {
        if (p_pos.row < 0 || p_pos.row >= m_map.size()) {
            return false;
        }

        if (p_pos.col < 0 || p_pos.col >= m_map[0].size()) {
            return false;
        }

        return m_map[p_pos.row][p_pos.col] == '.';
    }
};


class solution_infinite {
private:
    std::vector<std::string> m_map;
    int row_begin;
    int col_begin;

public:
    solution_infinite(const std::vector<std::string>& p_map, const int row, const int col) :
        m_map(p_map), row_begin(row), col_begin(col)
    { }

public:
    std::uint64_t solve(const std::uint64_t steps) const {
        std::uint64_t grids_amount = steps / m_map.size() - 1;

        std::uint64_t odd_grids = std::pow((grids_amount / 2 * 2) + 1, 2);
        std::uint64_t even_grids = std::pow((((grids_amount + 1) / 2) * 2), 2);

        std::uint64_t in_odd = solution(m_map).solve(m_map.size() * 2 + 1, row_begin, col_begin);
        std::uint64_t in_even = solution(m_map).solve(m_map.size() * 2, row_begin, col_begin);

        std::uint64_t in_corner_top = solution(m_map).solve(m_map.size() - 1, m_map.size() - 1, col_begin);
        std::uint64_t in_corner_right = solution(m_map).solve(m_map.size() - 1, row_begin, 0);
        std::uint64_t in_corner_bottom = solution(m_map).solve(m_map.size() - 1, 0, col_begin);
        std::uint64_t in_corner_left = solution(m_map).solve(m_map.size() - 1, row_begin, m_map.size() - 1);

        std::uint64_t in_edge_small_top_right = solution(m_map).solve(m_map.size() / 2 - 1, m_map.size() - 1, 0);
        std::uint64_t in_edge_small_top_left = solution(m_map).solve(m_map.size() / 2 - 1, m_map.size() - 1, m_map.size() - 1);
        std::uint64_t in_edge_small_bottom_right = solution(m_map).solve(m_map.size() / 2 - 1, 0, 0);
        std::uint64_t in_edge_small_bottom_left = solution(m_map).solve(m_map.size() / 2 - 1, 0, m_map.size() - 1);

        std::uint64_t in_edge_big_top_right = solution(m_map).solve(m_map.size() * 3 / 2 - 1, m_map.size() - 1, 0);
        std::uint64_t in_edge_big_top_left = solution(m_map).solve(m_map.size() * 3 / 2 - 1, m_map.size() - 1, m_map.size() - 1);
        std::uint64_t in_edge_big_bottom_right = solution(m_map).solve(m_map.size() * 3 / 2 - 1, 0, 0);
        std::uint64_t in_edge_big_bottom_left = solution(m_map).solve(m_map.size() * 3 / 2 - 1, 0, m_map.size() - 1);

        std::uint64_t full = odd_grids * in_odd + even_grids * in_even;
        std::uint64_t corners = in_corner_top + in_corner_right + in_corner_bottom + in_corner_left;
        std::uint64_t small_edges = (grids_amount + 1) * (in_edge_small_top_right + in_edge_small_top_left + in_edge_small_bottom_right + in_edge_small_bottom_left);
        std::uint64_t big_edges = grids_amount * (in_edge_big_top_right + in_edge_big_top_left + in_edge_big_bottom_right + in_edge_big_bottom_left);

        std::uint64_t result = full + corners + small_edges + big_edges;

        return result;
    }
};


std::vector<std::string> read_input() {
    std::ifstream stream("input.txt");

    std::vector<std::string> content;
    for (std::string line; std::getline(stream, line); ) {
        content.push_back(line);
    }

    return content;
}


int main() {
    auto map = read_input();

    int row_begin = 0;
    int col_begin = 0;

    for (int row = 0; row < map.size(); row++) {
        auto col = map[row].find('S');
        if (col != std::string::npos) {
            map[row][col] = '.';

            row_begin = row;
            col_begin = col;
            break;
        }
    }

    std::cout << "The number of garden spots (the Elf gave us favorite number): " << solution(map).solve(64, row_begin, col_begin) << std::endl;
    std::cout << "The number of garden spots (the Elf gave us steps): " << solution_infinite(map, row_begin, col_begin).solve(26501365) << std::endl;

    return 0;
}