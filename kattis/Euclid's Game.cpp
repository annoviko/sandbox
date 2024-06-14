#include <iostream>


bool is_stan_win(int a, int b) {
    if (a < b) {
        std::swap(a, b);
    }

    if (b == 0) {
        return false;   /* winner was on the prev step */
    }

    if (a % b == 0) {
        return true;    /* winner on the current step */
    }

    if (a / b >= 2) {
        return true;    /* the opponent is always in lose positive - the current play will win */
    }

    return !is_stan_win(a - (a / b) * b, b);    /* the best strategy is to keep the opponent within a / b < 2 */
}


int main() {
    int a, b;
    std::cin >> a >> b;

    while (a != 0 && b != 0) {
        if (is_stan_win(a, b)) {
            std::cout << "Stan wins" << std::endl;
        }
        else {
            std::cout << "Ollie wins" << std::endl;
        }

        std::cin >> a >> b;
    }

    return 0;
}