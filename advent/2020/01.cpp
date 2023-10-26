#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <unordered_set>


std::unordered_set<long long> read_input() {
    std::ifstream stream("Input.txt");

    std::unordered_set<long long> result;

    for (std::string line; std::getline(stream, line); ) {
        result.insert(std::stoi(line));
    }

    return result;
}


long long multiply_from_sum_two(const std::unordered_set<long long> & p_values, const long long target) {
    for (auto value : p_values) {	/* O(n) */
        auto difference = target - value;
        auto iter = p_values.find(difference);	/* O(1) */

        if (iter != p_values.end()) {
            return *iter * value;
        }
    }

    /* Total: O(n) */

    throw std::invalid_argument("Impossible for two numbers whose sum is '" + std::to_string(target) + "'.");
}


long long multiply_from_sum_three(const std::unordered_set<long long> & p_values, const long long target) {
    for (auto iter_first = std::begin(p_values); iter_first != std::end(p_values); iter_first++) {	/* O(n) */
        auto difference_target = target - *iter_first;

        if (difference_target < 0) { continue; }

        for (auto iter_second = std::begin(p_values); iter_second != std::end(p_values); iter_second++) {	/* O(n) */
            if (iter_second == iter_first) { continue; }

            auto difference = difference_target - *iter_second;
            auto iter_third = p_values.find(difference);	/* O(1) */
            if (iter_third != p_values.end()) {
                return *iter_first * *iter_second * *iter_third;
            }
        }
    }

    /* Total: O(n^2) */

    throw std::invalid_argument("Impossible for three numbers whose sum is '" + std::to_string(target) + "'.");
}


int main() {
    std::cout << "Multiply from sum of two 2020: " << multiply_from_sum_two(read_input(), 2020) << std::endl;
    std::cout << "Multiply from sum of three 2020: " << multiply_from_sum_three(read_input(), 2020) << std::endl;

    return 0;
}