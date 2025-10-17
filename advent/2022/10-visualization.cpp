#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <thread>
#include <variant>

#include <windows.h>


struct noop { };
struct addx {
    int value;
};


using command_t = std::variant<noop, addx>;


class solution {
private:
    std::vector<command_t> m_commands;

    const std::string CYCLE_COUNTER =   "CYCLE COUNTER:   ";
    const std::string COMMAND_COUNTER = "COMMAND COUNTER: ";
    const std::string COMMAND =         "COMMAND:         ";
    const std::string REGX =            "REGISTER X:      ";
    const int PANEL_ROW = 2;
    const int PANEL_COL = 3;

    const int ROW_OFFSET = 7;
    const int COL_OFFSET = 3;

public:
    solution(const std::vector<command_t>& p_commands) :
        m_commands(p_commands)
    { }

private:
    void move_cursor(int row, int col) {
        std::cout << "\033[" << row << ";" << col << "H";
    }

    void hide_cursor() {
        std::cout << "\033[?25l";
    }

    void show_cursor() {
        std::cout << "\033[?25h";
    }

    void draw_cell(int row, int col, char pixel) {
        move_cursor(row, col);
        if (pixel == '#') {
            std::cout << "\033[42m \033[0m";
        }
        else {
            std::cout << ' ';
        }
    }

    void draw_cursor(int row, int col) {
        move_cursor(row, col);
        std::cout << "\033[47m \033[0m";
    }

    void show_panel() {
        draw_panel_frame();

        move_cursor(PANEL_ROW, PANEL_COL + 2);
        std::cout << CYCLE_COUNTER;

        move_cursor(PANEL_ROW + 1, PANEL_COL + 2);
        std::cout << COMMAND_COUNTER;

        move_cursor(PANEL_ROW + 2, PANEL_COL + 2);
        std::cout << COMMAND;

        move_cursor(PANEL_ROW + 3, PANEL_COL + 2);
        std::cout << REGX;
    }

    void update_panel(int cycle, int command_counter, int regx, std::string& command) {
        move_cursor(PANEL_ROW, CYCLE_COUNTER.size() + 1);
        std::cout << cycle;

        move_cursor(PANEL_ROW + 1, COMMAND_COUNTER.size() + 1);
        std::cout << command_counter;

        move_cursor(PANEL_ROW + 2, COMMAND.size() + 1);
        std::cout << command;

        move_cursor(PANEL_ROW + 3, REGX.size() + 1);
        std::cout << std::to_string(regx) + std::string("    ");
    }

    void draw_panel_frame() {
        draw_frame(1, PANEL_COL, 4);
    }

    void draw_crt_frame() {
        draw_frame(ROW_OFFSET, COL_OFFSET, 6);
    }

    void draw_frame(int row_offset, int col_offset, int height) {
        move_cursor(row_offset, col_offset);
        std::cout << static_cast<char>(218) << std::string(40, static_cast<char>(196)) << static_cast<char>(191);

        for (int i = 0; i < height; i++) {
            move_cursor(row_offset + 1 + i, col_offset);
            std::cout << static_cast<char>(179);
            move_cursor(row_offset + 1 + i, col_offset + 41);
            std::cout << static_cast<char>(179);
        }

        move_cursor(row_offset + height + 1, col_offset);
        std::cout << static_cast<char>(192) << std::string(40, static_cast<char>(196)) << static_cast<char>(217);
    }

public:
    void visualization() {
        draw_crt_frame();
        hide_cursor();
        show_panel();

        int regx = 1;
        int regx_next = regx;
        int ticks = 0;
        int counter = 0;
        std::string command_name = "";

        int row = 0;
        int col = 0;

        for (int cycle = 0; cycle < 40 * 6; cycle++) {
            if (ticks == 0) {
                regx = regx_next;

                std::visit([&regx, &regx_next, &ticks, &command_name](auto&& c) {
                    using T = std::decay_t<decltype(c)>;

                    if constexpr (std::is_same_v<T, addx>) {
                        regx_next += c.value;
                        ticks = 2;
                        command_name = "ADDX";
                    }
                    else if constexpr (std::is_same_v<T, noop>) {
                        ticks = 1;
                        command_name = "NOOP";
                    }
                }, m_commands[counter]);

                counter++;
            }

            int x = cycle % 40;

            char pixel = (x <= regx + 1 && x >= regx - 1) ? '#' : ' ';
            draw_cell(ROW_OFFSET + 1 + row, COL_OFFSET + 1 + col, pixel);

            col++;
            if (col == 40) {
                row++;
                col = 0;
            }

            if (row < 6) {
                draw_cursor(ROW_OFFSET + 1 + row, COL_OFFSET + 1 + col);
            }

            update_panel(cycle, counter, regx, command_name);
            std::cout.flush();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            ticks--;
        }

        move_cursor(ROW_OFFSET + 15, 0);
    }
};


std::vector<command_t> read_input() {
    std::vector<command_t> result;

    std::fstream file("input.txt");
    for (std::string line; std::getline(file, line);) {
        std::stringstream ss(line);

        std::string name;
        ss >> name;

        if (name == "noop") {
            result.push_back(noop{});
        }
        else {
            addx command;
            ss >> command.value;
            result.push_back(command);
        }
    }

    return result;
}


int main() {
    auto input = read_input();

    solution(input).visualization();

    return 0;
}
