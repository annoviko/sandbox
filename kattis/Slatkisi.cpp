#include <iostream>


int main() {
    std::int64_t value;
    std::int64_t n;

    std::cin >> value >> n;

    std::int64_t multiplier = 1;
    std::int64_t next_multiplier = 10;
    for (int i = 0; i < n; i++) {
        int digit = (value % next_multiplier) / multiplier;

        value -= digit * multiplier;
        if (digit >= 5) {
            value += next_multiplier;
        }

        multiplier = next_multiplier;
        next_multiplier *= 10;
    }

    std::cout << value << std::endl;

    return 0;
}
