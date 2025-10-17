#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <variant>


struct noop { };
struct addx {
    int value;
};


using command_t = std::variant<noop, addx>;


class solution {
private:
    std::vector<command_t> m_commands;

public:
    solution(const std::vector<command_t>& p_commands) :
        m_commands(p_commands)
    { }

public:
    int signal_strength(const std::vector<int> p_query) {
        int index = 0;
        int regx_prev = 0;
        int regx = 1;
        int cycle = 1;
        int stregth = 0;

        for (int i = 0; (i < m_commands.size()) && (index < p_query.size()); i++) {
            std::visit([&regx, &regx_prev, &cycle](auto&& c) {
                using T = std::decay_t<decltype(c)>;

                if constexpr (std::is_same_v<T, addx>) {
                    regx_prev = regx;
                    regx += c.value;
                    cycle += 2;
                }
                else if constexpr (std::is_same_v<T, noop>) {
                    cycle++;
                }
            }, m_commands[i]);

            if (cycle == p_query[index]) {
                stregth += p_query[index] * regx;
                index++;
            }
            else if (cycle - 1 == p_query[index]) {
                stregth += p_query[index] * regx_prev;
                index++;
            }
        }

        return stregth;
    }

    std::vector<std::string> crt() {
        int regx = 1;
        int regx_next = regx;
        int ticks = 0;
        int counter = 0;

        std::vector<std::string> screen;
        std::string line;
        for (int cycle = 0; cycle < 40 * 6; cycle++) {
            if (ticks == 0) {
                regx = regx_next;

                std::visit([&regx, &regx_next, &ticks](auto&& c) {
                    using T = std::decay_t<decltype(c)>;

                    if constexpr (std::is_same_v<T, addx>) {
                        regx_next += c.value;
                        ticks = 2;
                    }
                    else if constexpr (std::is_same_v<T, noop>) {
                        ticks = 1;
                    }
                }, m_commands[counter]);

                counter++;
            }

            int x = cycle % 40;

            char pixel = (x <= regx + 1 && x >= regx - 1) ? '#' : '.';
            line.push_back(pixel);
            if (line.size() == 40) {
                screen.push_back(line);
                line.clear();
            }

            ticks--;
        }

        return screen;
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

    std::cout << "Signal strength: " << solution(input).signal_strength({ 20, 60, 100, 140, 180, 220 }) << std::endl;

    auto screen = solution(input).crt();
    std::cout << "CRT screen: " << std::endl;
    for (const auto& line : screen) {
        std::cout << line << std::endl;
    }

    return 0;
}
