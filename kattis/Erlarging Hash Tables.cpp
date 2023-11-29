#include <iostream>
#include <cmath>


bool is_prime(const std::uint64_t p_val) {   /* O(sqrt(N)) */
    if (p_val <= 1) {
        return false;
    }

    for (std::uint64_t i = 2; i <= std::sqrt(p_val); i++) {
        if (p_val % i == 0) {
            return false;
        }
    }

    return true;
}


int main() {
    while (true) {
        std::uint64_t n;
        std::cin >> n;

        if (n == 0) {
            break;
        }

        const bool is_size_prime = is_prime(n);
        std::uint64_t next_anchor = (n * 2) + 1;

        std::uint64_t candidate = next_anchor;
        for (; !is_prime(candidate); candidate++) {}    /* O(N * sqrt(N)) */

        std::cout << candidate;
        if (!is_size_prime) {
            std::cout << " (" << n << " is not prime)";
        }
        std::cout << std::endl;
    }

    return 0;
}