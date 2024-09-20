#include <iostream>
#include <cmath>
#include <cstdint>
#include <iterator>
#include <list>


int main() {
    std::uint64_t n = 0;
    std::cin >> n;

    std::list<std::uint64_t> dividers = { 1, n };
    std::uint64_t limit = std::sqrt((long double) n);

    auto middle = std::prev(dividers.end());
    for (std::uint64_t i = 2; i <= limit; i++) {
        if (n % i == 0) {
            dividers.insert(middle, i);

            std::uint64_t divider2 = n / i;
            if (i != divider2) {
                middle = dividers.insert(middle, divider2);
            }
        }
    }

    bool is_first = true;
    for (auto iter = dividers.begin(); iter != dividers.end(); iter++) {
        if (!is_first) {
            std::cout << ' ';
        }

        std::cout << *iter - 1;
        is_first = false;
    }

    std::cout << std::endl;

    return 0;
}