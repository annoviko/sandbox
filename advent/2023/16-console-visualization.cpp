#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <thread>
#include <chrono>
#include <unordered_map>

#include <conio.h>
#include <windows.h>


using namespace std::chrono_literals;


enum direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};


std::vector<std::string> read_input() {
    std::fstream stream("input.txt");

    std::vector<std::string> result;

    for (std::string line; std::getline(stream, line);) {
        result.push_back(line);
    }

    return result;
}


struct position {
    int row;
    int col;
};


struct beam {
    position p = { 0, 0 };
    direction dir = RIGHT;
};


class beam_tracker {
    std::vector<std::vector<std::vector<bool>>> cache;    /* [row][col][direction] */

    std::vector<std::vector<bool>> visited;
    std::vector<std::string> map;

    HANDLE std_handler = GetStdHandle(STD_OUTPUT_HANDLE);

public:
    beam_tracker(const std::vector<std::string>& _map) :
        map(_map)
    {
        cache = std::vector<std::vector<std::vector<bool>>>(map.size(), std::vector<std::vector<bool>>(map[0].size(), std::vector<bool>(4, false)));
        visited = std::vector<std::vector<bool>>(map.size(), std::vector<bool>(map[0].size(), false));

        int size = map.size() > map[0].size() ? map.size() : map[0].size();
        COORD buffer_size = { size + 10, size + 10 };  // Set your desired buffer size
        SetConsoleScreenBufferSize(std_handler, buffer_size);

        HWND console_window = GetConsoleWindow();

        CONSOLE_FONT_INFOEX font_info;
        font_info.cbSize = sizeof(CONSOLE_FONT_INFOEX);
        font_info.nFont = 0;
        font_info.dwFontSize.X = 8;
        font_info.dwFontSize.Y = 9;
        font_info.FontFamily = FF_DONTCARE;
        font_info.FontWeight = FW_NORMAL;
        wcscpy_s(font_info.FaceName, L"Consolas");

        SetCurrentConsoleFontEx(std_handler, FALSE, &font_info);

        int window_width = (font_info.dwFontSize.X + 1) * map[0].size();
        int window_height = (font_info.dwFontSize.Y + 1) * map.size();

        RECT r;
        GetWindowRect(console_window, &r);
        MoveWindow(console_window, 50, 50, window_width, window_height, TRUE);

        CONSOLE_CURSOR_INFO cursor_info;
        GetConsoleCursorInfo(std_handler, &cursor_info);
        cursor_info.bVisible = false;
        SetConsoleCursorInfo(std_handler, &cursor_info);
    }

public:
    void visualize() {
        _getch();

        show_original();
        _getch();

        CONSOLE_CURSOR_INFO cursor_info;
        GetConsoleCursorInfo(std_handler, &cursor_info);
        cursor_info.bVisible = false;
        SetConsoleCursorInfo(std_handler, &cursor_info);

        analyse_basic_configuration();
    }

private:
    void show_original() {
        SetConsoleCursorPosition(std_handler, { 0, 0 });
        SetConsoleTextAttribute(std_handler, 7);

        for (auto& r : map) {
            std::cout << r << std::endl;
        }
    }


    void show_beam(const int row, const int col) {
        SetConsoleCursorPosition(std_handler, { (short)col, (short)row });

        SetConsoleTextAttribute(std_handler, 207);
        std::cout << map[row][col];
        SetConsoleTextAttribute(std_handler, 7);
    }


    void show_visited(const int row, const int col) {
        SetConsoleCursorPosition(std_handler, { (short)col, (short)row });

        SetConsoleTextAttribute(std_handler, 12);
        std::cout << map[row][col];
        SetConsoleTextAttribute(std_handler, 7);
    }


    std::int64_t analyse_basic_configuration() {
        return analyse(beam{ { 0, -1 }, RIGHT });
    }

    std::int64_t analyse(const beam& initial) {
        std::queue<beam> to_process;
        to_process.push(initial);

        std::int64_t counter = 0;
        std::int64_t frames = 0;

        while (true) {
            std::queue<beam> next_queue;

            while (!to_process.empty()) {
                beam cur = to_process.front();
                to_process.pop();

                auto next_position = get_next_position(cur);
                if (!is_inside(next_position.row, next_position.col)) {
                    if (is_inside(cur.p.row, cur.p.col)) {
                        show_visited(cur.p.row, cur.p.col);
                    }

                    continue;
                }

                show_beam(next_position.row, next_position.col);

                if (is_inside(cur.p.row, cur.p.col)) {
                    show_visited(cur.p.row, cur.p.col);
                }

                cur.p = next_position;
                char action = map[next_position.row][next_position.col];
                switch (action) {
                    case '.': {
                        next_queue.push(cur);
                        break;
                    }

                    case '/':
                    case '\\': {
                        cur.dir = mirror_beam(cur, action);
                        next_queue.push(cur);

                        break;
                    }

                    case '-':
                    case '|': {
                        auto new_beams = split_beam(cur, action);
                        for (auto& new_beam : new_beams) {
                            next_queue.push(new_beam);
                        }
                    }
                }
            }

            to_process = std::move(next_queue);
            std::this_thread::sleep_for(20ms);
        }

#if 0
#if 0
#define stream std::cout
#else
        std::ofstream stream("out.txt");
#endif
        stream << std::endl;
        for (auto r : visited) {
            for (auto v : r) {
                if (v) {
                    stream << '#';
                }
                else {
                    stream << '.';
                }
            }

            stream << std::endl;
        }
#endif
        return counter;
    }

private:
    void clean_cache() {
        cache = std::vector<std::vector<std::vector<bool>>>(map.size(), std::vector<std::vector<bool>>(map[0].size(), std::vector<bool>(4, false)));
        visited = std::vector<std::vector<bool>>(map.size(), std::vector<bool>(map[0].size(), false));
    }

    bool is_inside(int r, int c) {
        return r >= 0 && r < map.size() && c >= 0 && c < map[0].size();
    }

    std::vector<beam> split_beam(const beam& b, char splitter) {
        switch (splitter) {
        case '-': return split_beam_horizontal(b);
        case '|': return split_beam_vertical(b);
        }
    }

    std::vector<beam> split_beam_horizontal(const beam& b) {
        switch (b.dir) {
        case RIGHT:
        case LEFT:
            return { b };

        case UP:
        case DOWN:
            return { { b.p, LEFT }, { b.p, RIGHT } };
        }
    }

    std::vector<beam> split_beam_vertical(const beam& b) {
        switch (b.dir) {
        case UP:
        case DOWN:
            return { b };

        case RIGHT:
        case LEFT:
            return { { b.p, UP }, { b.p, DOWN } };
        }
    }

    direction mirror_beam(const beam& b, char mirror) {
        switch (mirror) {
        case '/': return mirror_beam_forward_slash(b);
        case '\\': return mirror_beam_back_slash(b);
        }
    }

    direction mirror_beam_forward_slash(const beam& b) {
        switch (b.dir) {
        case RIGHT: return UP;
        case DOWN:  return LEFT;
        case LEFT:  return DOWN;
        case UP:    return RIGHT;
        }
    }

    direction mirror_beam_back_slash(const beam& b) {
        switch (b.dir) {
        case RIGHT: return DOWN;
        case DOWN:  return RIGHT;
        case LEFT:  return UP;
        case UP:    return LEFT;
        }
    }

    position get_next_position(beam& b) {
        switch (b.dir) {
        case UP:
            return { b.p.row - 1, b.p.col };

        case DOWN:
            return { b.p.row + 1, b.p.col };

        case RIGHT:
            return { b.p.row, b.p.col + 1 };

        case LEFT:
            return { b.p.row, b.p.col - 1 };
        }
    }
};


int main() {
    auto input = read_input();

    beam_tracker(input).visualize();

    return 0;
}
