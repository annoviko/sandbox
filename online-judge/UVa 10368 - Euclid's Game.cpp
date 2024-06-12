#include <iostream>


bool is_stan_win(int a, int b) {
    if (a < b) {
        std::swap(a, b);
    }

    if (b == 0) {
        return false;
    }

    if (a % b == 0) {
        return true;
    }

    if (a / b >= 2) {
        return true;
    }

    return !is_stan_win(a % b, b);
}


int main() {
    int a, b;
    std::cin >> a >> b;

    while (a != 0 && b != 0) {
        bool stan_win = is_stan_win(a, b);
        if (stan_win) {
            std::cout << "Stan wins" << std::endl;
        }
        else {
            std::cout << "Ollie wins" << std::endl;
        }

        std::cin >> a >> b;
    }

    return 0;
}