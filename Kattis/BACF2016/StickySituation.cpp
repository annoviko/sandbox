#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

std::vector<std::size_t> get_sticks() {
    std::vector<std::size_t> sticks;

    std::size_t amount = 0;
    std::cin >> amount;

    for (std::size_t i = 0; i < amount; i++) {
        std::size_t length = 0;
        std::cin >> length;
        sticks.push_back(length);
    }

    return sticks;
}

bool is_non_degerate_triangle_buildable(const std::vector<std::size_t> & p_sticks) {
    for (std::size_t i = 2; i < p_sticks.size(); i++) {
        if (p_sticks[i - 2] + p_sticks[i - 1] > p_sticks[i]) {
            return true;
        }
    }

    return false;
}

int main() {
    auto sticks = get_sticks();
    std::sort(sticks.begin(), sticks.end());

    if (is_non_degerate_triangle_buildable(sticks)) {
        std::cout << "possible" << std::endl;
    }
    else {
        std::cout << "impossible" << std::endl;
    }

    return 0;
}