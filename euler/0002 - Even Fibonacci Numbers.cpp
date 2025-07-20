#include <iostream>


int main() {
    std::uint64_t a1 = 1;
    std::uint64_t a2 = 2;

    std::uint64_t sum = 2;
    const std::uint64_t lim = 4000000;

    int cycle_counter = 0;
    for (std::uint64_t i = 3, an = 3; an < lim; i++, an = a1 + a2, cycle_counter++) {
        if (an % 2 == 0) {
            sum += an;
        }

        a1 = a2;
        a2 = an;
    }

    std::cout << sum << " (cycles: " << cycle_counter << ") \t- [brute force for testing]" << std::endl;

    cycle_counter = 0;
    a1 = 2;
    a2 = 8;
    sum = a1 + a2;

    for (std::uint64_t an = a2 * 4 + a1; an < lim; an = a2 * 4 + a1, cycle_counter++) {
        sum += an;
        a1 = a2;
        a2 = an;
    }

    std::cout << sum << " (cycles: " << cycle_counter << ") \t- [optimized formula]" << std::endl;

    return 0;
}
