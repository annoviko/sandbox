#include <iostream>
#include <cmath>

int main() {
    long int k;
    std::cin >> k;

    long int v = std::ceil(static_cast<double>(k) / 2.0);
    long int h = std::floor(static_cast<double>(k) / 2.0);

    long int n = (v + 1) * (h + 1);

    std::cout << n << std::endl;

    return 0;
}