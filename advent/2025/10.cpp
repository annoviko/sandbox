#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>


const int NOT_FOUND = std::numeric_limits<int>::max();


struct machine_t {
    std::string pattern;
    std::vector<std::string> buttons;
    std::vector<int> joltage;
};


class solution {
private:
    std::vector<machine_t> m_factory;

    std::unordered_map<std::string, int> cache;
    std::unordered_set<std::string> visited;
    int m_cur = 0;

public:
    solution(const std::vector<machine_t>& f) : m_factory(f) { }

public:
    int min_buttons_to_configure() {
        int counter = 0;
        for (m_cur = 0; m_cur < m_factory.size(); m_cur++) {
            visited.clear();
            cache.clear();

            const std::string state = std::string(m_factory[m_cur].pattern.size(), '.');
            const int result_for_machine = min_buttons(state, -1);

            counter += result_for_machine;
        }
        return counter;
    }

private:
    int min_buttons(std::string state, int button_index) {
        if (state == m_factory[m_cur].pattern) {
            cache[state] = 1;   /* nothing can be faster */
            return 1;
        }

        {
            auto iter = cache.find(state);
            if (iter != cache.cend()) {
                return iter->second;
            }
        }

        {
            auto iter = visited.find(state);
            if (iter != visited.cend()) {
                return NOT_FOUND;  /* we were in this state - consider it as no way */
            }
        }

        if (button_index != -1) {
            visited.insert(state);
        }

        int best_result = NOT_FOUND;
        for (int i = 0; i < m_factory[m_cur].buttons.size(); i++) {
            const std::string next_state = press(state, i);
            const int candidate = min_buttons(next_state, i);
            if (candidate == NOT_FOUND) {
                continue;   /* path with no success */
            }

            best_result = std::min(best_result, candidate);
        }

        if (best_result != NOT_FOUND) {
            if (button_index != -1) {
                best_result++;  /* count currently pressed button */
            }

            auto iter = cache.find(state);
            if (iter != cache.cend()) {
                iter->second = std::min(best_result, iter->second);
            }
            else {
                cache[state] = best_result;
            }
        }

        return best_result;
    }

    std::string press(const std::string& state, const int btn_idx) {
        std::string new_state = state;
        const auto& buttons = m_factory[m_cur].buttons[btn_idx];

        for (int i = 0; i < buttons.size(); i++) {
            const int idx = buttons[i] - '0';
            new_state[idx] = (state[idx] == '#') ? '.' : '#';
        }

        return new_state;
    }
};


std::vector<machine_t> read_input() {
    std::fstream stream("input.txt");
    std::vector<machine_t> input;

    for (std::string line; std::getline(stream, line); ) {
        machine_t machine;

        std::size_t p = line.find(']', 1);
        machine.pattern = line.substr(1, p - 1);

        p = line.find('(', p);
        std::string button;
        while (p != std::string::npos) {
            button.push_back(line[p + 1]);
            p += 2;

            if (line[p] == ')') {
                machine.buttons.push_back(std::move(button));
                p = line.find('(', p);
            }
        }

        p = line.find('{');
        std::stringstream ss(line.substr(p + 1));

        int value = -1;
        char ignore = 0;
        while (ss >> value) {
            machine.joltage.push_back(value);
            ss >> ignore;
        }

        input.push_back(machine);
    }

    return input;
}


int main() {
    auto input = read_input();

    std::cout << "Minimum buttons to press: " << solution(input).min_buttons_to_configure() << std::endl;

    return 0;
}