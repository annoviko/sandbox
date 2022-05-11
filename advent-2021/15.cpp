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
    path_finder(const cave_map& p_map) :
        m_map(p_map)
    {
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

    void traverse() {
        m_map.back().back() = 0;
        m_cost.back().back() = 0;

        std::list<neighbor_position> to_visit;
        to_visit.push_back({ m_map.size() - 1, m_map[0].size() - 1 });

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

public:
    std::uint64_t find_optimal_way() {
        traverse();
        return m_cost[0][0];
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

    std::cout << "The smallest risk path: " << path_finder(input).find_optimal_way() << std::endl;

    return 0;
}
