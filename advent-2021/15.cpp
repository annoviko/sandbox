#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>


using cave_row = std::vector<char>;
using cave_map = std::vector<cave_row>;

using cost_row = std::vector<std::uint64_t>;
using cost_map = std::vector<cost_row>;

using visit_row = std::vector<bool>;
using visit_map = std::vector<visit_row>;


struct neighbor_position {
    std::uint64_t row = 0;
    std::uint64_t col = 0;
};


class path_finder {
private:
    cave_map m_map;
    cost_map m_cost;
    visit_map m_visit;

public:
    path_finder(const cave_map& p_map, bool p_multi_tile) {
        if (!p_multi_tile) {
            m_map = p_map;
        }
        else {
            cave_map big_map = cave_map(p_map.size() * 5, cave_row(p_map[0].size() * 5, 0));

            std::size_t row_tile = 0;
            for (std::size_t i = 0; i < big_map.size(); i++) {
                if ((i != 0) && (i % p_map.size() == 0)) {
                    row_tile++;
                }

                std::size_t col_tile = 0;
                for (std::size_t j = 0; j < big_map[0].size(); j++) {
                    if ((j != 0) && (j % p_map[0].size() == 0)) {
                        col_tile++;
                    }

                    const std::uint64_t i_small = i % p_map.size();
                    const std::uint64_t j_small = j % p_map[0].size();

                    std::uint64_t value_to_add = p_map[i_small][j_small] + row_tile + col_tile;
                    while(value_to_add > 9) {
                        value_to_add -= 9;
                    }

                    big_map[i][j] = value_to_add;
                }
            }

            m_map = std::move(big_map);
        }

        m_cost = cost_map(m_map.size(), cost_row(m_map[0].size(), std::numeric_limits<std::uint64_t>::max()));
        m_visit = visit_map(m_map.size(), visit_row(m_map[0].size(), false));
    }

private:
    bool is_finish(const std::uint64_t p_row, const std::uint64_t p_col) {
        return (p_row == 0) && (p_col == 0);
    }


    bool is_visited(const std::uint64_t p_row, const std::uint64_t p_col) {
        return m_visit[p_row][p_col];
    }


    std::list<neighbor_position> get_non_visited_neighbors(std::uint64_t p_row, std::uint64_t p_col) {
        std::list<neighbor_position> neighbors;
        if ((p_row > 0) && !m_visit[p_row - 1][p_col]) {
            neighbors.push_back({ p_row - 1, p_col });
        }

        if ((p_col > 0) && !m_visit[p_row][p_col - 1]) {
            neighbors.push_back({ p_row, p_col - 1 });
        }

        if ((p_row + 1 < m_map.size()) && !m_visit[p_row + 1][p_col]) {
            neighbors.push_back({ p_row + 1, p_col });
        }

        if ((p_col + 1 < m_map[0].size()) && !m_visit[p_row][p_col + 1]) {
            neighbors.push_back({ p_row, p_col + 1 });
        }

        return neighbors;
    }

    void traverse(std::uint64_t i_start, std::uint64_t j_start) {
        std::list<neighbor_position> to_visit;
        to_visit.push_back({ i_start, j_start });

        while(!to_visit.empty()) {
            auto node = to_visit.front(); // get closest node
            to_visit.pop_front();

            if (is_visited(node.row, node.col)) {
                continue; // we might have non-unique neighbors in the list since it is list
            }

            auto neighbors = get_non_visited_neighbors(node.row, node.col);

            for (auto& neighbor : neighbors) {
                auto cost_to_move = m_map[neighbor.row][neighbor.col] + m_cost[node.row][node.col];
                if (cost_to_move < m_cost[neighbor.row][neighbor.col]) {
                    m_cost[neighbor.row][neighbor.col] = cost_to_move;
                }

                to_visit.push_back(neighbor);
            }

            m_visit[node.row][node.col] = true;
            to_visit.sort([this](const neighbor_position& a, const neighbor_position& b) {
                return m_cost[a.row][a.col] < m_cost[b.row][b.col];
            });
        }
    }

    void print_cost() {
        for (const auto& row : m_cost) {
            for (const auto val : row) {
                if (val != std::numeric_limits<decltype(val)>::max()) {
                    std::cout << std::setw(3) << val << " ";
                }
                else {
                    std::cout << "INF ";
                }
            }

            std::cout << std::endl;
        }
    }

    void print_map() {
        for (const auto& row : m_map) {
            for (const auto val : row) {
                std::cout << (int) val;
            }

            std::cout << std::endl;
        }
    }

public:
    std::uint64_t find_optimal_way_in_tile() {
        m_map.back().back() = 0;
        m_cost.back().back() = 0;

        traverse(m_map.size() - 1, m_map[0].size() - 1);

        return m_cost.front().front();
    }


    std::uint64_t find_optimal_way_in_map() {
        m_map.front().front() = 0;
        m_cost.front().front() = 0;

        traverse(0, 0);

        return m_cost.back().back();
    }
};


cave_map read_input() {
    std::ifstream stream("input.txt");

    cave_map result;
    for (std::string str_row; std::getline(stream, str_row);) {
        cave_row row;
        for (const char digit : str_row) {
            row.push_back(digit - '0');
        }

        result.push_back(row);
    }

    return result;
}


int main() {
    cave_map input = read_input();

    std::cout << "The smallest risk path in the tile: " << path_finder(input, false).find_optimal_way_in_tile() << std::endl;
    std::cout << "The smallest risk path in the complete map: " << path_finder(input, true).find_optimal_way_in_map() << std::endl;

    return 0;
}
