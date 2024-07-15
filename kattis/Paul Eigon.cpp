#include <iostream>
#include <cstdint>


int main() {
    std::uint64_t n, p, q;
    std::cin >> n >> p >> q;

    std::uint64_t d = (p + q) / n;
    std::cout << ((d % 2 == 0) ? "paul" : "opponent") << std::endl;

    return 0;
}
