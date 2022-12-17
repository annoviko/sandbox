#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


std::vector<int> read() {
    std::ifstream stream("input.txt");

    std::vector<int> result;
    int current_value = 0;
    for (std::string line; std::getline(stream, line);) {
        if (line.empty()) {
            result.push_back(current_value);
            current_value = 0;
            continue;
        }

        current_value += std::stoi(line);
    }

    if (current_value > 0) {
        result.push_back(current_value);
    }

    return result;
}


int main() {
    auto elf_bags = read();

    std::cout << "Maximum calories in the bag: " << *std::max_element(elf_bags.begin(), elf_bags.end()) << std::endl;

    std::sort(elf_bags.begin(), elf_bags.end(), std::greater<>());
    int calories = 0;
    for (int i = 0; i < 3; i++) {
        calories += elf_bags[i];
    }

    std::cout << "Calories from the three biggest bags: " << calories << std::endl;

    return 0;
}