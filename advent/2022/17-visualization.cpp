#include <algorithm>
#include <fstream>
#include <cstdint>
#include <iostream>
#include <list>
#include <string>
#include <sstream>
#include <vector>
#include <limits>
#include <windows.h>


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

    void visualize_stones(std::uint64_t count) {
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD console_mode = 0;
        if (GetConsoleMode(console, &console_mode)) {
            SetConsoleMode(console, console_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
        CONSOLE_CURSOR_INFO cursor{};
        GetConsoleCursorInfo(console, &cursor);
        const bool was_visible = cursor.bVisible;
        cursor.bVisible = FALSE;
        SetConsoleCursorInfo(console, &cursor);

        /* clear the prompt once; subsequent frames are drawn in place */
        CONSOLE_SCREEN_BUFFER_INFO screen_info{};
        GetConsoleScreenBufferInfo(console, &screen_info);
        const COORD origin{ 0, 0 };
        const DWORD console_size = screen_info.dwSize.X * screen_info.dwSize.Y;
        DWORD written = 0;
        FillConsoleOutputCharacterA(console, ' ', console_size, origin, &written);
        FillConsoleOutputAttribute(console, screen_info.wAttributes,
                                   console_size, origin, &written);

        int iteration = 0;
        for (std::uint64_t i = 0; i < count; ++i) {
            iteration = simulate(static_cast<int>(i % FIGURES.size()), iteration,
                                 console, i + 1, count, static_cast<int>(i % FIGURES.size()));
        }

        render({}, console, count, count, 0, iteration);
        std::cout << "\nDone. Press Enter to exit..." << std::flush;
        std::cin.get();
        cursor.bVisible = was_visible;
        SetConsoleCursorInfo(console, &cursor);
    }

private:
    int simulate(int index, int iteration, HANDLE console = nullptr,
                 std::uint64_t completed = 0, std::uint64_t total = 0,
                 int shape_index = 0) {
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

        if (console) render(coord, console, completed, total, shape_index, iteration);

        int previous_row = -1;
        while (previous_row != coord[0].r) {
            previous_row = coord[0].r;

            int dc = get_dc(iteration);

            move_figure_by_jet(dc, coord);
            move_figure_by_gravity(coord);

            if (console) render(coord, console, completed, total, shape_index, iteration);

            iteration++;
        }

        std::uint64_t new_height = coord[0].r + 1;    /* 0 - the top of the figure */
        height = max(height, new_height);

        for (const auto& p : coord) {
            field[p.r][p.c] = true;
        }

        if (console) render(coord, console, completed, total, shape_index, iteration, true);

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

    void render(const std::vector<position_t>& figure, HANDLE console,
                std::uint64_t completed, std::uint64_t total,
                int shape_index, int iteration, bool settling = false) {
        std::ostringstream frame;
        constexpr const char* reset = "\x1b[0m";
        constexpr const char* cyan = "\x1b[36m";
        constexpr const char* yellow = "\x1b[93m";
        constexpr const char* blue = "\x1b[94m";
        constexpr const char* gray = "\x1b[90m";
        constexpr const char* green = "\x1b[92m";
        static const char* shape_names[] = {
            "Horizontal line", "Plus", "Reverse L", "Vertical line", "Square"
        };
        const std::string clear_to_end_of_line(70, ' ');

        std::uint64_t settled_blocks = 0;
        for (const auto& row : field) {
            for (bool block : row) {
                settled_blocks += block ? 1 : 0;
            }
        }

        frame << cyan << "AoC 2022 - Pyroclastic Flow" << reset
              << clear_to_end_of_line << "\r\n"
              << "Falling rocks: " << completed << " / " << total
              << "    Tower height: " << height << clear_to_end_of_line << "\r\n"
              << clear_to_end_of_line << "\r\n";

        constexpr int viewport_height = 24;
        const int top = max(0, static_cast<int>(field.size()) - viewport_height);
        for (int row = viewport_height - 1; row >= 0; --row) {
            const int i = top + row;
            frame << cyan << '|' << reset;
            if (i < static_cast<int>(field.size())) {
                for (int j = 0; j < static_cast<int>(field[i].size()); ++j) {
                    bool active_figure = false;
                    bool settling_figure = false;
                    for (const auto& p : figure) {
                        if (p.r == i && p.c == j) {
                            active_figure = true;
                            settling_figure = settling;
                            break;
                        }
                    }
                    if (active_figure) {
                        if (settling_figure) {
                            frame << green << '#' << reset;
                        }
                        else {
                            frame << yellow << '@' << reset;
                        }
                    }
                    else {
                        frame << (field[i][j] ? blue : gray)
                              << (field[i][j] ? '#' : '.') << reset;
                    }
                }
            } else {
                frame << gray << "......." << reset;
            }
            frame << cyan << "|" << reset;
            if (row == viewport_height - 1) {
                frame << "    " << yellow << "STATISTICS" << reset;
            }
            else if (row == viewport_height - 3) {
                frame << "    Rock:        " << completed << " / " << total;
            }
            else if (row == viewport_height - 4) {
                frame << "    Shape:       " << shape_names[shape_index];
            }
            else if (row == viewport_height - 5) {
                frame << "    Height:      " << height;
            }
            else if (row == viewport_height - 6) {
                frame << "    Jet step:    " << iteration;
            }
            else if (row == viewport_height - 7) {
                frame << "    Settled:     " << settled_blocks;
            }
            else if (row == viewport_height - 8) {
                frame << "    Visible top: " << top;
            }
            frame << clear_to_end_of_line << "\r\n";
        }

        frame << cyan << "+-------+" << reset << "\r\n"
              << "Legend: " << yellow << "@" << reset << " falling  "
              << blue << "#" << reset << " settled  "
              << gray << "." << reset << " empty" << clear_to_end_of_line << "\r\n";
        const std::string text = frame.str();
        const COORD origin{ 0, 0 };
        DWORD written = 0;
        SetConsoleCursorPosition(console, origin);
        WriteConsoleA(console, text.data(), static_cast<DWORD>(text.size()),
                      &written, nullptr);
        Sleep(settling ? 180 : 53);
    }
};


int main() {
    auto jets = read_input();

    std::uint64_t number_of_rocks = 0;
    std::cout << "How many rocks should be visualized? ";
    std::cin >> number_of_rocks;
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

    if (number_of_rocks > 0) {
        solution(jets).visualize_stones(number_of_rocks);
        return 0;
    }

    return 0;
}
