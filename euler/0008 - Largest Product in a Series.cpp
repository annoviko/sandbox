#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::string value;

    std::ifstream file("input.txt");
    for (std::string line; std::getline(file, line);) {
        value += line;
    }

    int digits = 13;

    std::uint64_t product = 0;
    std::uint64_t max = 0;
    std::uint64_t len = 0;
    for (int i = 0; i < value.size(); i++) {
        std::uint64_t cur_value = value[i] - '0';
        if (cur_value == 0) {
            len = 0;
            continue;
        }

        if (len == 0) {
            product = cur_value;
        }
        else if (len < digits) {
            product *= cur_value;
        }
        else {
            std::uint64_t first = value[i - digits] - '0';
            product = (product / first) * cur_value;
        }

        len++;
        if (len >= digits) {
            max = std::max(product, max);
        }
    }

    std::cout << max << std::endl;
    return 0;
}