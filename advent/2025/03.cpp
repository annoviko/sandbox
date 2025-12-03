#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <string>


class solution {
private:
    std::vector<std::string> m_banks;

public:
    solution(const std::vector<std::string>& p_banks) :
        m_banks(p_banks)
    { }

public:
    std::uint64_t largest_total_joltage(const int count) {
        std::uint64_t total = 0;
        for (const std::string& line : m_banks) {
            std::vector<char> joltage;
            for (int i = line.size() - 1; i > line.size() - 1 - count; i--) {
                joltage.push_back(line[i]);
            }
            std::reverse(joltage.begin(), joltage.end());

            for (int i = line.size() - 1 - count; i >= 0; i--) {
                int cur = line[i];
                for (int k = 0; k < joltage.size() && cur >= joltage[k]; k++) {
                    int next_cur = joltage[k];
                    joltage[k] = cur;
                    cur = next_cur;
                }
            }

            std::uint64_t joltage_value = 0;

            for (char v : joltage) {
                joltage_value = joltage_value * 10 + (v - '0');
            }

            total += joltage_value;
        }

        return total;
    }
};


std::vector<std::string> read_input() {
    std::fstream stream("input.txt");
    std::vector<std::string> batteries;

    for (std::string line; std::getline(stream, line);) {
        batteries.push_back(line);
    }

    return batteries;
}


int main() {
    const auto input = read_input();

    std::cout << "The total output joltage (2 batteries): " << solution(input).largest_total_joltage(2) << std::endl;
    std::cout << "The total output joltage (12 batteries): " << solution(input).largest_total_joltage(12) << std::endl;

    return 0;
}