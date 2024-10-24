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
    {
        for (int row = 0; row < m_map.size(); row++) {
            auto col = m_map[row].find('S');
            if (col != std::string::npos) {
                m_map[row][col] = '.';
                m_pos[row].insert(col);
                break;
            }
        }
    }

public:
    std::uint64_t solve(const int steps) {
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
        if (p_pos.row < 0 && p_pos.row >= m_map.size()) {
            return false;
        }

        if (p_pos.col < 0 && p_pos.col >= m_map[0].size()) {
            return false;
        }

        return m_map[p_pos.row][p_pos.col] == '.';
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

    std::cout << "The number of garden spots: " << solution(map).solve(64) << std::endl;

    return 0;
}