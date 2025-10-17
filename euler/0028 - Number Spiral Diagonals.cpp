#include <iostream>
#include <cmath>


int main() {
    const int len = 1001;
    static_assert(len % 2 == 1, "len must be odd");

    std::uint64_t result = 1;
    for (int i = 0; i < len / 2; i++) {
        const std::uint64_t delta = 2 * (i + 1);

        const std::uint64_t tr = std::pow(3 + i * 2, 2);
        const std::uint64_t tl = tr - delta;
        const std::uint64_t bl = tl - delta;
        const std::uint64_t br = bl - delta;

        result += tr + tl + bl + br;
    }

    std::cout << result << std::endl;

    return 0;
}