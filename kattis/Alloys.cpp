#include <iostream>


int main() {
    double long c;
    std::cin >> c;

    if (c > 1.0) {
        c = 1.0;    /* x + y + z = 1 */
    }

    /* max(x * y) is when x = y, so x = y = c / 2 */
    const double long x = c / 2.0;
    const double long y = x;

    const double long xy = x * y;

    std::cout << xy << std::endl;
    return 0;
}