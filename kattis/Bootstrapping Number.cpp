#include <iostream>
#include <iomanip>
#include <cmath>


int main() {
    long double l = 1, r = 10;

    long double target;
    std::cin >> target;

    long double delta = 1;
    long double mid = 0;

    do {
        mid = (l + r) / 2.0;
        long double current = std::pow(mid, mid);

        delta = std::abs(current - target);
        if (current > target) {
            r = mid;
        }
        else {
            l = mid;
        }
    } while (delta > 0.0000001);

    std::cout << std::setprecision(15) << mid << std::endl;

    return 0;
}