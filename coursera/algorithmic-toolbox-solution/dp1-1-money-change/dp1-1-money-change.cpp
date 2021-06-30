#include <iostream>
#include <vector>
#include <algorithm>

const std::vector<int> COINS = { 1, 3, 4 };

int get_change(int m) {
    std::vector<int> dp(m + 1, 0);
    for (std::size_t i = 1; i < dp.size(); i++) {
        int coins = std::numeric_limits<int>::max();
        for (const auto & value : COINS) {
            if (i >= value) {
                int candidate = dp[i - value] + 1;
                coins = std::min(coins, candidate);
            }
        }

        dp[i] = coins;
    }

    return dp.back();
}

void check(int n, int expected) {
    if (get_change(n) != expected) {
        std::cout << "FAILURE: " << n << std::endl;
    }
    else {
        std::cout << "SUCCESS" << std::endl;
    }
}

void test() {
    check(1, 1);
    check(2, 2);
    check(3, 1);
    check(4, 1);
    check(5, 2);
    check(6, 2);
    check(7, 3);
    check(8, 2);
    check(9, 3);
    check(10, 4);
    check(12, 3);
}

int main() {
    int m;
    std::cin >> m;
    std::cout << get_change(m) << '\n';
}
