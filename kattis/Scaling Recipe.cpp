#include <cstdint>
#include <iostream>


int main() {
    std::int64_t n, x, y;
    std::cin >> n >> x >> y;

    for (std::int64_t i = 0; i < n; i++) {
        std::int64_t value;
        std::cin >> value;

        std::int64_t result = (y * value) / x;
        std::cout << result << std::endl;
    }

    return 0;
}
