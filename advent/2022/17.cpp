#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>


#if 0
    #define CONSOLE_VISUALIZATION
    #include <conio.h>
#endif


struct position_t {
    int r = 0;
    int c = 0;
};


const std::vector<std::vector<position_t>> FIGURES = {
    { { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 } },
    { { 0, 1 }, { 1, 0 }, { 1, 1 }, { 1, 2 }, { 2, 1 } },
    { { 0, 2 }, { 1, 2 }, { 2, 2 }, { 2, 1 }, { 2, 0 } },
    { { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 } },
    { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } }
};


const std::vector<int> HEIGHTS = { 1, 3, 3, 4, 2 };


std::string read_input() {
    std::ifstream stream("input.txt");
    
    std::string jets;
    std::getline(stream, jets);

    return jets;
}


class solution {
private:
    std::string jets;
    std::vector<std::vector<bool>> field;

    std::uint64_t height = 0;

public:
    solution(const std::string& j) : jets(j) { }

    std::uint64_t tower_height_for_2022_stones() {
        int iteration = 0;

        for (int i = 0; i < 2022; i++) {
            const int index_figure = i % FIGURES.size();
            iteration = simulate(index_figure, iteration);
        }

        return height;
    }

    std::uint64_t tower_height_for_1000000000000_stones() {
        int iteration = 0;

        const std::uint64_t verification_steps = 2;
        const std::uint64_t max_size = 50;    /* pattern size */
        std::list<std::uint64_t> pattern;
        std::list<std::uint64_t> current;

        std::uint64_t loop_begins_at = 2022;  /* assume that loops will have a place after 2022 figures */
        std::uint64_t loop_size = 0;
        std::uint64_t loops_counter = 0;

        std::uint64_t previous_height = 0;
        std::uint64_t current_figure_index = 0;

        std::vector<std::uint64_t> loop_elements;
        std::uint64_t loop_total_height = 0;

        /* find loop size and its prefix */
        for (std::uint64_t i = 0; i < 1000000000000; i++) {
            const std::uint64_t index_figure = i % FIGURES.size();
            iteration = simulate(index_figure, iteration);

            std::uint64_t delta = height - previous_height;
            previous_height = height;

            if (i >= 2022) {
                if (pattern.size() < max_size) {
                    pattern.push_back(delta);
                }
                else if (current.size() < max_size) {
                    current.push_back(delta);
                }
                else {
                    current.pop_front();
                    current.push_back(delta);
                }

                if (loops_counter == verification_steps - 1) {
                    loop_elements.push_back(delta);
                    loop_total_height += delta;
                }

                if (pattern == current) {
                    std::uint64_t detected_loop_size = i - loop_begins_at - pattern.size();
                    if (loop_size == detected_loop_size) {
                        loops_counter++;
                        if (loops_counter == verification_steps) {
                            current_figure_index = i + 1;
                            break;
                        }
                    }
                    else {
                        loops_counter = 0;
                    }

                    loop_size = detected_loop_size;
                    loop_begins_at = i - pattern.size();

                    current.clear();
                }
            }
        }

        /* compute height */
        std::uint64_t remaining_figures = 1000000000000 - current_figure_index;
        loops_counter = remaining_figures / loop_size;
        remaining_figures = remaining_figures % loop_size;

        std::uint64_t total_height = height + loops_counter * loop_total_height;
        for (std::uint64_t i = 0; i < remaining_figures; i++) {
            total_height += loop_elements[i];
        }

        return total_height;
    }


private:
    int simulate(int index, int iteration) {
        int height_to_add = height + (3 + HEIGHTS[index]) - field.size();
        for (int i = 0; i < height_to_add; i++) {
            field.push_back(std::vector<bool>(7, false));
        }

        const int height_to_start = height + 3 + HEIGHTS[index];
        const int dr = field.size() - height_to_start;
        const int dc = 2;

        /* place figure to initial position */
        std::vector<position_t> coord = FIGURES[index];
        for (int i = 0; i < coord.size(); i++) {
            coord[i].c += dc;
            coord[i].r = (field.size() - 1) - coord[i].r - dr;
        }

#if defined(CONSOLE_VISUALIZATION)
        visualize(coord);
#endif

        int previous_row = -1;
        while (previous_row != coord[0].r) {
            previous_row = coord[0].r;

            int dc = get_dc(iteration);

            move_figure_by_jet(dc, coord);
            move_figure_by_gravity(coord);

#if defined(CONSOLE_VISUALIZATION)
            visualize(coord);
#endif

            iteration++;
        }

        std::uint64_t new_height = coord[0].r + 1;    /* 0 - the top of the figure */
        height = std::max(height, new_height);

        for (const auto& p : coord) {
            field[p.r][p.c] = true;
        }

#if defined(CONSOLE_VISUALIZATION)
        visualize({});
#endif

        return iteration;
    }

    int get_dc(int iteration) {
        int jet_index = iteration % jets.size();
        char jet = jets[jet_index];

        if (jet == '<') {
            return -1;
        }
        
        return 1;
    }

    void move_figure_by_jet(int dc, std::vector<position_t>& p) {
        move_figure({ 0, dc }, p);
    }

    void move_figure_by_gravity(std::vector<position_t>& p) {
        move_figure({ -1, 0 }, p);
    }

    void move_figure(const position_t& d, std::vector<position_t>& p) {
        bool success = true;
        std::vector<position_t> next_coord = p;
        for (int i = 0; i < next_coord.size(); i++) {
            int row = next_coord[i].r + d.r;
            int col = next_coord[i].c + d.c;

            if ((col < 0) || (col >= field[0].size()) || (row < 0) || (field[row][col] != false)) {
                success = false;
                break;
            }

            next_coord[i].r = row;
            next_coord[i].c = col;
        }

        if (success) {
            p = next_coord;
        }
    }

#if defined(CONSOLE_VISUALIZATION)
    void visualize(const std::vector<position_t>& figure) {
        system("cls");
        std::cout << "Height: " << height << "\n\n";

        for (int i = field.size() - 1; i >= 0; i--) {
            std::cout << '|';
            for (int j = 0; j < field[i].size(); j++) {
                bool active_figure = false;
                for (const auto& p : figure) {
                    if (p.r == i && p.c == j) {
                        active_figure = true;
                        std::cout << '@';
                        break;
                    }
                }

                if (!active_figure) {
                    std::cout << (field[i][j] ? '#' : '.');
                }
            }
            std::cout << '|' << std::endl;
        }

        std::cout << "+-------+" << std::endl;
        _getch();
    }
#endif
};


int main() {
    auto jets = read_input();

    std::cout << "The height of the tower of rocks (2022):          " << solution(jets).tower_height_for_2022_stones() << std::endl;
    std::cout << "The height of the tower of rocks (1000000000000): " << solution(jets).tower_height_for_1000000000000_stones() << std::endl;

    return 0;
}