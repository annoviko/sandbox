#include <cmath>
#include <iostream>
#include <vector>

#if 0
bool is_prime(const int value) {
    if ((value == 2) || (value == 3)) {
        return true;
    }

    if ((value % 2 == 0) || (value % 3 == 0)) {
        return false;
    }

    const int limit = std::sqrt(value);
    for (int i = 5; i <= limit; i += 2) {
        if (value % i == 0) {
            return false;
        }
    }

    return true;
}


std::int64_t sum_prime(const int limit) {
    std::int64_t sum = 0;
    for (int i = 2; i < limit; i++) {   /* O(N) */
        if (is_prime(i)) {  /* O(sqrt(N)) */
            sum += i;
        }
    }

    return sum; /* O(N * sqrt(N)) */
}
#else
std::int64_t sum_prime(const int limit) {
    std::vector<std::uint8_t> prime_table(limit, true);
    prime_table[0] = prime_table[1] = false;

    int sqrt_limit = std::sqrt(limit);
    for (int i = 2; i < sqrt_limit; i++) {  /* O(sqrt(N)) */
        if (prime_table[i]) {
            for (int j = i * i; j < limit; j += i) {    /* O(log(log(N))) */
                prime_table[j] = false;
            }
        }
    }

    std::int64_t sum = 0;
    for (int i = 0; i < prime_table.size(); i++) {
        if (prime_table[i]) {
            sum += i;
        }
    }

    return sum; /* O(sqrt(N) * log(log(N))) */
}
#endif


int main() {
    int limit = 2000000;
    std::cout << "The sum of the primes below " << limit << ": " << sum_prime(limit) << std::endl;
    return 0;
}