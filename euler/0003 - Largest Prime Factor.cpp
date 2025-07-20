#include <iostream>
#include <cmath>


bool is_prime(std::int64_t value) {
    for (std::int64_t i = 2; i <= (std::int64_t)std::sqrt(value); i++) {
        if (value % i == 0) {
            return false;
        }
    }

    return true;
}


int main() {
#if 0
    std::int64_t value = 13195;
#else
    std::int64_t value = 600851475143;
#endif
    std::int64_t prime = -1;

    for (std::int64_t i = 2; i <= (std::int64_t)std::sqrt(value); i++) {
        if (value % i == 0) {
            if (is_prime(i)) {
                prime = i;
            }
        }
    }

    if (prime == -1) {
        std::cout << value << std::endl;
    }
    else {
        std::cout << prime << std::endl;
    }

    return 0;
}