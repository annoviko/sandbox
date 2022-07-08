#include <algorithm>
#include <vector>


class Solution {
public:
    int lengthOfLIS(const std::vector<int>& nums) {
        std::vector<int> rnums = nums;
        std::reverse(rnums.begin(), rnums.end());

        std::vector<int> dp(rnums.size(), 0);
        for (std::size_t i = 0; i < rnums.size(); i++) {
             for (std::size_t j = i + 1; j < rnums.size(); j++) {
                if (rnums[i] > rnums[j]) {
                    dp[j] = std::max(dp[i] + 1, dp[j]);
                }
            }
        }

        return *std::max_element(dp.begin(), dp.end()) + 1;
    }
};
