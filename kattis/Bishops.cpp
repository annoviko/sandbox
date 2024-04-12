#include <iostream>


int main() {
    int n = 0;
    while (std::cin >> n) {
        if (n < 0) {
            break;
        }

        if (n > 1) {
            std::cout << (n + n - 2) << '\n';
        }
        else {
            std::cout << n << '\n';
        }
    }

    return 0;
}