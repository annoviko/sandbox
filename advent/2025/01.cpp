#include <fstream>
#include <iostream>
#include <string>
#include <vector>


std::vector<int> read_input() {
    std::vector<int> actions;

    std::fstream stream("input.txt");
    std::string value;
    while (stream >> value) {
        const int move = std::stoi(value.substr(1));
        actions.push_back({ value[0] == 'R' ? move : -move });
    }

    return actions;
}


class solution {
private:
    std::vector<int> m_instr;

public:
    solution(const std::vector<int>& p_instr) : m_instr(p_instr) {}

public:
    int get_password_legacy() {
        int counter = 0;
        int position = 50;

        for (int act : m_instr) {
            position += act;
            position %= 100;

            if (position < 0) {
                position += 100;
            }
            else if (position == 0) {
                counter++;
            }
        }

        return counter;
    }

    int get_password_0x434C49434B() {
        int counter = 0;
        int position = 50;
        int previous = 50;

        for (int act : m_instr) {
            counter += std::abs(act) / 100;
            act %= 100;
            if (act == 0) {
                previous = position;
                continue;
            }

            position += act;

            if (position >= 100) {
                counter++;
                position -= 100;
            }
            else if (position < 0) {
                if (previous != 0) {
                    counter++;
                }

                position += 100;
            }
            else if (position == 0) {
                counter++;
            }

            previous = position;
        }

        return counter;
    }
};


int main() {
    auto actions = read_input();

    std::cout << "Actual password: " << solution(actions).get_password_legacy() << std::endl;
    std::cout << "Actual password (method 0x434C49434B): " << solution(actions).get_password_0x434C49434B() << std::endl;

    return 0;
}