#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;

int lcs2(vector<int> &a, vector<int> &b) {
    std::vector<std::vector<int>> dp(a.size() + 1, std::vector<int>(b.size() + 1));

    for (int i = 0; i < a.size() + 1; i++) {
        for (int j = 0; j < b.size() + 1; j++) {
            int value = 0;

            if (i > 0) {
                value = std::max(dp[i - 1][j], value);
            }

            if (j > 0) {
                value = std::max(dp[i][j - 1], value);
            }

            if (i > 0 && j > 0) {
                int cost = ((i - 1 < a.size()) && (j - 1 < b.size()) && (a[i - 1] == b[j - 1])) ? 1 : 0;
                value = std::max(dp[i - 1][j - 1] + cost, value);
            }

            dp[i][j] = value;
        }
    }

    return dp.back().back();
}

int main() {
    size_t n;
    std::cin >> n;
    vector<int> a(n);
    for (size_t i = 0; i < n; i++) {
        std::cin >> a[i];
    }

    size_t m;
    std::cin >> m;
    vector<int> b(m);
    for (size_t i = 0; i < m; i++) {
        std::cin >> b[i];
    }

    std::cout << lcs2(a, b) << std::endl;
}
