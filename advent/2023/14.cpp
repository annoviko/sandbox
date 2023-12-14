#include <iostream>
#include <fstream>
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

public:
    solution(const std::vector<std::string>& _plt) : 
        map(_plt)
    { }

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

    std::int64_t count_north_load_after_spin_cycle() {
        auto fix_point = map;
        
        /* find loop*/
        std::int64_t actual_simulation_steps = 0;
        std::int64_t loop_size = 0;
        while (true) {
            spin();

            loop_size++;
            actual_simulation_steps++;

            if (fix_point == map) {
                break;
            }

            if (loop_size > 100) {
                /* stabilization point is not found yet - set new fix point */
                fix_point = map;
                loop_size = 0;
            }
        }

        /* find steps to get final state */
        std::int64_t remaining_steps = 1000000000 - actual_simulation_steps;
        remaining_steps = remaining_steps % loop_size;

        /* simulate until final step */
        for (int i = 0; i < remaining_steps; i++) {
            spin();
        }

        /* count result */
        std::int64_t result = 0;
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

    void print() {
        std::cout << std::endl;

        for (auto& r : map) {
            std::cout << r << std::endl;
        }

        std::cout << std::endl;
    }

private:
    void spin() {
        tilt_north();
        tilt_west();
        tilt_south();
        tilt_east();
    }

    void tilt_north() {
        for (int r = 0; r < map.size(); r++) {
            for (int c = 0; c < map[r].size(); c++) {
                if (map[r][c] == 'O') {
                    map[r][c] = '.';

                    int current_free = r;
                    for (int i = current_free; i >= 0 && map[i][c] != '#'; i--) {
                        if (map[i][c] == '.') {
                            current_free = i;
                        }
                    }

                    map[current_free][c] = 'O';
                }
            }
        }
    }

    void tilt_south() {
        for (int r = map.size() - 1; r >= 0; r--) {
            for (int c = 0; c < map[r].size(); c++) {
                if (map[r][c] == 'O') {
                    map[r][c] = '.';

                    int current_free = r;
                    for (int i = current_free; i < map.size() && map[i][c] != '#'; i++) {
                        if (map[i][c] == '.') {
                            current_free = i;
                        }
                    }

                    map[current_free][c] = 'O';
                }
            }
        }
    }

    void tilt_west() {
        for (int c = 0; c < map[0].size(); c++) {
            for (int r = 0; r < map.size(); r++) {
                if (map[r][c] == 'O') {
                    map[r][c] = '.';

                    int current_free = c;
                    for (int i = current_free; i >= 0 && map[r][i] != '#'; i--) {
                        if (map[r][i] == '.') {
                            current_free = i;
                        }
                    }

                    map[r][current_free] = 'O';
                }
            }
        }
    }

    void tilt_east() {
        for (int c = map[0].size() - 1; c >= 0; c--) {
            for (int r = 0; r < map.size(); r++) {
                if (map[r][c] == 'O') {
                    map[r][c] = '.';

                    int current_free = c;
                    for (int i = current_free; i < map[0].size() && map[r][i] != '#'; i++) {
                        if (map[r][i] == '.') {
                            current_free = i;
                        }
                    }

                    map[r][current_free] = 'O';
                }
            }
        }
    }
};


int main() {
    auto input = read_input();



    std::cout << "The total load on the north support beams: " << solution(input).count_north_load() << std::endl;
    std::cout << "The total load on the north support beams after spin cycle: " << solution(input).count_north_load_after_spin_cycle() << std::endl;

    return 0;
}