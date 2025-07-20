#include <iostream>
#include <numeric>

int main() {
    std::uint64_t values = 20;
    std::uint64_t cur_lcm = 2;
    for (std::uint64_t i = 2; i < values; i++) {
        cur_lcm = std::lcm(cur_lcm, i + 1);
    }

    std::cout << cur_lcm << std::endl;

    return 0;
}
