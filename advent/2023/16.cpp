#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <unordered_map>


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


class solution {
    std::vector<std::vector<std::vector<bool>>> cache;    /* [row][col][direction] */

    std::vector<std::vector<bool>> visited;
    std::vector<std::string> map;

public:
    solution(const std::vector<std::string>& _map) : map(_map) {
        cache = std::vector<std::vector<std::vector<bool>>>(map.size(), std::vector<std::vector<bool>>(map[0].size(), std::vector<bool>(4, false)));
        visited = std::vector<std::vector<bool>>(map.size(), std::vector<bool>(map[0].size(), false));
    }

public:
    std::int64_t analyse() {
        std::queue<beam> to_process;
        to_process.push(beam{ { 0, -1 }, RIGHT });

        std::int64_t counter = 0;

        while (!to_process.empty()) {
            beam cur = to_process.front();
            to_process.pop();

            auto next_position = get_next_position(cur);
            if (!is_inside(next_position.row, next_position.col)) {
                continue;
            }

            if (!visited[next_position.row][next_position.col]) {
                counter++;
            }
            visited[next_position.row][next_position.col] = true;

            cur.p = next_position;
            char action = map[next_position.row][next_position.col];
            switch (action) {
                case '.': {
                    if (!cache[cur.p.row][cur.p.col][cur.dir]) {
                        to_process.push(cur);
                    }
                    cache[cur.p.row][cur.p.col][cur.dir] = true;

                    break;
                }

                case '/':
                case '\\': {
                    cur.dir = mirror_beam(cur, action);

                    if (!cache[cur.p.row][cur.p.col][cur.dir]) {
                        to_process.push(cur);
                    }
                    cache[cur.p.row][cur.p.col][cur.dir] = true;

                    break;
                }

                case '-':
                case '|': {
                    auto new_beams = split_beam(cur, action);
                    for (auto& new_beam : new_beams) {
                        if (cache[new_beam.p.row][new_beam.p.col][new_beam.dir]) {
                            continue;
                        }

                        to_process.push(new_beam);
                        cache[new_beam.p.row][new_beam.p.col][new_beam.dir] = true;
                    }
                }
            }
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

public:
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

    std::cout << "Energized tiles: " << solution(input).analyse() << std::endl;

    return 0;
}
