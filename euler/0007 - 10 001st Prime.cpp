#include <iostream>
#include <cmath>


bool is_prime(const std::int64_t n) {
    if (n % 2 == 0) { return false; }
    if (n % 3 == 0) { return false; }

    std::int64_t sqrt_n = std::sqrt(n);
    for (int i = 2; i <= sqrt_n; i++) {
        if (n % i == 0) {
            return false;
        }
    }

    return true;
}


int main() {
    std::int64_t n = 10001;
    
    std::int64_t prime = -1;
    std::int64_t count = 0;

    for (prime = 2; count != n; prime++) {
        if (is_prime(prime)) {
            count++;
        }
    }

    prime--;
    std::cout << n << "th prime number: " << prime << std::endl;

    return 0;
}