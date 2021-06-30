#include <iostream>
#include <cstdlib>

long long lcm_naive(int a, int b) {
    for (long l = 1; l <= (long long)a * b; ++l)
        if (l % a == 0 && l % b == 0)
            return l;

    return (long long)a * b;
}

long long gcd_fast(int a, int b) {
    long long large = (a >= b) ? a : b;
    long long small = (a < b) ? a : b;

    while (small != 0) {
        long long r = large % small;
        large = small;
        small = r;
    }

    return large;
}

long long lcm_fast(int a, int b) {
    return (static_cast<long long>(a) * static_cast<long long>(b)) / gcd_fast(a, b);
}

void stress_test() {
    while (true) {
        int value1 = std::rand() % 100 + 1;
        int value2 = std::rand() % 100 + 1;

        if (lcm_fast(value1, value2) != lcm_naive(value1, value2)) {
            std::cout << "FAIL: " << value1 << ", " << value2 << std::endl;
        }
        else {
            std::cout << "OK" << std::endl;
        }
    }
}

int main() {
    // stress_test();

    int a, b;
    std::cin >> a >> b;
    std::cout << lcm_fast(a, b) << std::endl;
    return 0;
}
