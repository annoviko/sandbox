#include <iostream>

int arithmetic_progression(int d, int limit) {
    int n = (limit - 1) / d;
    int a0 = d;
    int an = d * n;

    return (a0 + an) * n / 2;
}

int main() {
    int lim = 1000;
    int sum = arithmetic_progression(3, lim) + arithmetic_progression(5, lim) - arithmetic_progression(15, lim);
    std::cout << sum << std::endl;
    return 0;
}
