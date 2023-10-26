#include <fstream>
#include <iostream>
#include <list>
#include <cstdint>

std::list<std::int64_t> read_values() {
    std::list<std::int64_t> result;

    std::ifstream infile("input.txt");
    std::int64_t value;
    while (infile >> value) {
        result.push_back(value);
    }
    infile.close();

    return result;
}


std::int64_t get_total_fuel() {
    auto masses = read_values();
    long long result = 0;
    for (auto mass : masses) {
        mass = mass / 3 - 2;
        while (mass >= 0) {
            result += mass;
            mass = mass / 3 - 2;
        }
    }

    return result;
}


int main() {
    std::cout << "Result: " << get_total_fuel() << std::endl;

    return 0;
}