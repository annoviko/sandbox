#include <iostream>
#include <vector>


int main() {
    const int n = 20;

    std::vector<std::uint64_t> dp(n + 1, 1);

    for (int i = 1; i < n + 1; i++) {
        std::vector<std::uint64_t> dp_next(n + 1, 0);
        for (int j = 0; j < n + 1; j++) {
            dp_next[j] = dp[j] + ((j > 0) ? dp_next[j - 1] : 0);
        }
        dp = std::move(dp_next);
    }

    std::cout << dp.back() << std::endl;
    return 0;
}