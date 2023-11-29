#include <iostream>


int main() {
    std::uint64_t a, b;
    while (std::cin >> a >> b) {
        std::uint64_t diff = (a > b) ? a - b : b - a;
        std::cout << diff << std::endl;
    }

    return 0;
}