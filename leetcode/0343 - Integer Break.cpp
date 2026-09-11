#include <vector>


class Solution {
public:
#if 1   /* DP solution with math observation - O(N) */
    int integerBreak(int n) {
        std::vector<int> dp(n + 1, 1);

        /* 
           Math observation: try to get 3 * 3 * 3 * ... as much as possible, fills others with 2
           Essentially this is an expession: x < 3 * (x - 3)
           And all x's are in DP already.

           - 11: 3 * 3 * 3 * 2 = 54
           - 11: 2 * 2 * 2 * 2 * 2 * 1 = 32
           - 11: 4 * 3 * 3 * 1 = 36

           So the strategy here is to try get as much as possible 3 rather than 2.
           
        */
        for (int i = 3; i <= n; i++) {
            dp[i] = i - 1;
            for (int j = 2; (j < i) && (j <= 3); j++) {      /* { 2, 3 } */
                const int candidate1 = (i - j) * j;
                const int candidate2 = dp[i - j] * j;

                dp[i] = std::max(dp[i], candidate1);
                dp[i] = std::max(dp[i], candidate2);
            }
        }

        return dp.back();
    }
#else   /* DP solution - O(N^2) - acceptable - beats 100% (mostly because N <= 58) */
    int integerBreak(int n) {
        std::vector<int> dp(n + 1, 1);
        for (int i = 2; i <= n; i++) {
            for (int j = 1; j <= i / 2; j++) {
                const int candidate1 = (i - j) * j;
                const int candidate2 = dp[i - j] * j;

                dp[i] = std::max(dp[i], candidate1);
                dp[i] = std::max(dp[i], candidate2);
            }
        }

        return dp.back();
    }
#endif
};