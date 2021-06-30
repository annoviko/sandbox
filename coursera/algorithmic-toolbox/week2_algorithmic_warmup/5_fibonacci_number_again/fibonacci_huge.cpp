#include <iostream>
#include <cstdlib>

long long get_fibonacci_huge_naive(long long n, long long m) {
    if (n <= 1)
        return n;

    long long previous = 0;
    long long current = 1;

    for (long long i = 0; i < n - 1; ++i) {
        long long tmp_previous = previous;
        previous = current;
        current = tmp_previous + current;
    }

    return current % m;
}

long long get_fibonacci_huge_fast(long long n, long long m) {
    if (n <= 1) {
        return n;
    }

    long long previous = 0;
    long long current = 1;

    long long period = 0;

    do {
        long long next = current + previous;
        previous = current;
        current = next % m;
        period++;
    } 
    while (previous != 0 || current != 1);

    n = n % period;
    if (n == 0) {
        return 0;   // no need to search for number
    }

    previous = 0, current = 1;

    for (int i = 1; i < n; i++) {
        long long next = current + previous;
        previous = current;
        current = next % m;
    }

    return current;
}

void stress_test() {
    while (true) {
        long long n = std::rand() % 10 + 1;
        long long m = std::rand() % 10 + 2;

        if (get_fibonacci_huge_fast(n, m) != get_fibonacci_huge_naive(n, m)) {
            std::cout << "FAIL: " << n << ", " << m << std::endl;
            std::terminate();
        }
        else {
            std::cout << "OK" << std::endl;
        }
    }
}

int main() {
    //stress_test();

    long long n, m;
    std::cin >> n >> m;
    std::cout << get_fibonacci_huge_fast(n, m) << '\n';
    //std::cout << get_fibonacci_huge_naive(n, m) << '\n';
}
