#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>


struct position {
    std::int64_t row;
    std::int64_t col;
};


std::vector<std::string> read_universe() {
    std::fstream stream("input.txt");

    std::vector<std::string> result;

    for (std::string line; std::getline(stream, line);) {
        result.push_back(line);
    }

    return result;
}


class universe_analyser {
private:
    std::vector<std::string> g;

    std::vector<position> galaxies;
    std::vector<position> actual_galaxies;

    std::vector<std::int64_t> rows_to_grow;
    std::vector<std::int64_t> cols_to_grow;

public:
    universe_analyser(const std::vector<std::string>& _g) : g(_g) {
        galaxies = find_galaxies();
        rows_to_grow = find_rows_without_galaxies();
        cols_to_grow = find_cols_without_galaxies();
    }

public:
    std::int64_t shortest_path_between_galaxies(const std::int64_t expansion) {
        actual_galaxies = get_galaxies_position_after_expansion(expansion);

        std::int64_t sum = 0;

        std::int64_t pair = 0;
        for (std::int64_t i = 0; i < actual_galaxies.size(); i++) {
            for (std::int64_t j = i + 1; j < actual_galaxies.size(); j++) {
                std::int64_t candidate = manhattan(actual_galaxies[i], actual_galaxies[j]);

                sum += candidate;
            }
        }

        return sum;
    }

private:
    std::vector<position> get_galaxies_position_after_expansion(const std::int64_t expansion) {
        std::vector<position> result;
        for (std::int64_t i = 0; i < galaxies.size(); i++) {
            result.push_back(get_galaxy_position(galaxies[i], expansion));
        }
        return result;
    }


    position get_galaxy_position(const position& p, const std::int64_t expansion) {
        std::int64_t row_offset = 0;
        for (std::int64_t i = 0; i < rows_to_grow.size(); i++) {
            if (p.row > rows_to_grow[i]) {
                row_offset += expansion;
            }
        }

        std::int64_t col_offset = 0;
        for (std::int64_t i = 0; i < cols_to_grow.size(); i++) {
            if (p.col > cols_to_grow[i]) {
                col_offset += expansion;
            }
        }

        return { p.row + row_offset, p.col + col_offset };
    }


    std::vector<position> find_galaxies() {
        std::vector<position> result;

        for (std::int64_t r = 0; r < g.size(); r++) {
            for (std::int64_t c = 0; c < g[r].size(); c++) {
                if (g[r][c] == '#') {
                    result.push_back({ r, c });
                }
            }
        }

        return result;
    }


    std::vector<std::int64_t> find_rows_without_galaxies() {
        std::vector<std::int64_t> result;

        for (std::int64_t r = 0; r < g.size(); r++) {
            if (g[r].find('#') == std::string::npos) {
                result.push_back(r);
            }
        }

        return result;
    }


    std::vector<std::int64_t> find_cols_without_galaxies() {
        std::vector<std::int64_t> result;

        for (std::int64_t c = 0; c < g[0].size(); c++) {
            bool galaxy_found = false;
            for (std::int64_t r = 0; r < g.size(); r++) {
                if (g[r][c] == '#') {
                    galaxy_found = true;
                    break;
                }
            }

            if (!galaxy_found) {
                result.push_back(c);
            }
        }

        return result;
    }


    std::int64_t manhattan(const position& p1, const position& p2) {
        return std::abs(p1.row - p2.row) + std::abs(p1.col - p2.col);
    }
};


int main() {
    std::vector<std::string> universe = read_universe();

    std::cout << "The sum of shortest length between galaxies with (1 expansion): " << universe_analyser(universe).shortest_path_between_galaxies(1) << std::endl;
    std::cout << "the sum of shortest length between galaxies with (999999 expansion): " << universe_analyser(universe).shortest_path_between_galaxies(999999) << std::endl;

    return 0;
}