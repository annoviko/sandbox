#include <iostream>
#include <cmath>

int main() {
    std::uint64_t n = 100;

    std::uint64_t s1 = (2 * std::pow(n, 3) + 3 * std::pow(n, 2) + n) / 6;
    std::uint64_t s2 = std::pow(n * (2 + (n - 1)) / 2, 2);

    std::cout << s2 - s1 << std::endl;

    return 0;
}