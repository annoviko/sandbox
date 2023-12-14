#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <string>


std::vector<std::string> read_input() {
    std::fstream stream("input.txt");

    std::vector<std::string> result;
    for (std::string line; std::getline(stream, line);) {
        result.push_back(line);
    }

    return result;
}


class solution {
    std::vector<std::string> map;

    std::vector<std::vector<int>> north_to_rock;    /* points to the first free index below rock */

public:
    solution(const std::vector<std::string>& _plt) : 
        map(_plt), north_to_rock(_plt.size())
    {
        build_north_to_rock();
    }

public:
    std::int64_t count_north_load() {
        std::int64_t result = 0;

        tilt_north();

        for (int row = 0; row < map.size(); row++) {
            std::int64_t rocks = 0;
            for (int col = 0; col < map[row].size(); col++) {
                if (map[row][col] == 'O') {
                    rocks++;
                }
            }

            result += rocks * (map.size() - row);
        }

        return result;
    }

private:
    void tilt_north() {
        for (int r = map.size() - 1; r >= 0; r--) {
            for (int c = 0; c < map[r].size(); c++) {
                if (map[r][c] == 'O') {
                    map[r][c] = '.';

                    int row_below_rock = north_to_rock[r][c];
                    while (map[row_below_rock][c] != '.') {
                        row_below_rock++;
                    }

                    map[row_below_rock][c] = 'O';
                }
            }
        }
    }

    void build_north_to_rock() {
        std::vector<int> current_rocks(map.size(), 0);

        for (int r = 0; r < map.size(); r++) {
            if (north_to_rock[r].empty()) {
                north_to_rock[r] = std::vector<int>(map[r].size());
            }

            for (int c = 0; c < map[r].size(); c++) {
                if (map[r][c] == '#') {
                    current_rocks[c] = r + 1;
                    north_to_rock[r][c] = -1;
                }
                else {
                    north_to_rock[r][c] = current_rocks[c];
                }
            }
        }
    }
};


int main() {
    auto input = read_input();

    std::cout << "The total load on the north support beams: " << solution(input).count_north_load() << std::endl;

    return 0;
}