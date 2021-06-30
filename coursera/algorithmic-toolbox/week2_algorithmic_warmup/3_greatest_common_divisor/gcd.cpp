#include <iostream>
#include <cstdlib>

int gcd_naive(int a, int b) {
    int current_gcd = 1;
    for (int d = 2; d <= a && d <= b; d++) {
        if (a % d == 0 && b % d == 0) {
            if (d > current_gcd) {
                current_gcd = d;
            }
        }
    }
    return current_gcd;
}

int gcd_fast(int a, int b) {
    int large = (a >= b) ? a : b;
    int small = (a < b) ? a : b;

    while (small != 0) {
        int r = large % small;
        large = small;
        small = r;
    }

    return large;
}

void stress_test() {
    while (true) {
        int value1 = std::rand() % 1000 + 1;
        int value2 = std::rand() % 1000 + 1;

        if (gcd_naive(value1, value2) != gcd_fast(value1, value2)) {
            std::cout << "FAIL: " << value1 << ", " << value2 << std::endl;
            std::terminate();
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
    std::cout << gcd_fast(a, b) << std::endl;
    return 0;
}
