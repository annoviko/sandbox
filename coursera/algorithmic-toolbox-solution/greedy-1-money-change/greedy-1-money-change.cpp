#include <iostream>
#include <vector>
#include <string>

const std::vector<int> COINS = { 1, 5, 10 };

int get_suitable_coin(const int p_value) {
    for (auto it = COINS.crbegin(); it != COINS.crend(); it++) {
        if (p_value - *it >= 0) {
            return *it;
        }
    }

    throw std::out_of_range("Impossible to find suitable coin for '" + std::to_string(p_value) + "'.");
}

int get_change(int m) {
    int amount = 0;

    while (m > 0) {
        m -= get_suitable_coin(m);
        amount++;
    }

    return amount;
}

int main() {
    int m;
    std::cin >> m;
    std::cout << get_change(m) << std::endl;
}
