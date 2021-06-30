#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;


int lcs3(vector<int> &a, vector<int> &b, vector<int> &c) {
    std::vector<std::vector<std::vector<int>>> dp(a.size() + 1, 
        std::vector<std::vector<int>>(b.size() + 1, 
            std::vector<int>(c.size() + 1, 0)));

    for (int i = 0; i < a.size() + 1; i++) {
        for (int j = 0; j < b.size() + 1; j++) {
            for (int k = 0; k < c.size() + 1; k++) {
                int value = 0;

                if (i > 0) {
                    value = std::max(dp[i - 1][j][k], value);
                }

                if (j > 0) {
                    value = std::max(dp[i][j - 1][k], value);
                }

                if (i > 0 && j > 0) {
                    value = std::max(dp[i - 1][j - 1][k], value);
                }

                if (k > 0) {
                    value = std::max(dp[i][j][k - 1], value);
                }

                if (i > 0 && k > 0) {
                    value = std::max(dp[i - 1][j][k - 1], value);
                }

                if (j > 0 && k > 0) {
                    value = std::max(dp[i][j - 1][k - 1], value);
                }

                if (i > 0 && j > 0 && k > 0) {
                    int cost = ((i - 1 < a.size()) && (j - 1 < b.size()) && (k - 1 < c.size()) && 
                        (a[i - 1] == b[j - 1]) && (b[j - 1] == c[k - 1])) ? 1 : 0;

                    value = std::max(dp[i - 1][j - 1][k - 1] + cost, value);
                }

                dp[i][j][k] = value;
            }
        }
    }

    return dp.back().back().back();
}

int main() {
    size_t an;
    std::cin >> an;
    vector<int> a(an);
    for (size_t i = 0; i < an; i++) {
        std::cin >> a[i];
    }
    size_t bn;
    std::cin >> bn;
    vector<int> b(bn);
    for (size_t i = 0; i < bn; i++) {
        std::cin >> b[i];
    }
    size_t cn;
    std::cin >> cn;
    vector<int> c(cn);
    for (size_t i = 0; i < cn; i++) {
        std::cin >> c[i];
    }
    std::cout << lcs3(a, b, c) << std::endl;
}
