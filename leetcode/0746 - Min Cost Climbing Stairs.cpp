#include <vector>


class Solution {
public:
#if 0
    int minCostClimbingStairs(const std::vector<int>& cost) {
        std::vector<int> dp(cost.size());
        
        for (std::size_t i = 0; i < cost.size(); i++) {          
            int left = (i > 0) ? dp[i - 1] + cost[i] : cost[i];
            int right = (i > 1) ? dp[i - 2] + cost[i] : cost[i];

            dp[i] = std::min(left, right);
        }
        
        int left = (dp.size() > 0) ? dp[dp.size() - 1] : 0;
        int right = (dp.size() > 1) ? dp[dp.size() - 2] : 0;
        return std::min(left, right);
    }
#else
    int minCostClimbingStairs(const std::vector<int>& cost) {
        int dp_prev_prev = cost[0];  // 2 <= cost.length <= 1000
        int dp_prev = cost[1];
        
        for (std::size_t i = 2; i < cost.size(); ++i) {
            const int current = std::min(dp_prev + cost[i], dp_prev_prev + cost[i]);
            dp_prev_prev = dp_prev;
            dp_prev = current;
        }
        
        return std::min(dp_prev_prev, dp_prev);
    }
#endif
};