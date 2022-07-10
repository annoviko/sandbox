class Solution {
public:
    int jump(const std::vector<int>& nums) {
        if (nums.size() == 1) {
            return 0;
        }

        int dp = nums[0];
        int interval_end = dp + 1;
        int cost = 1;

        for (std::size_t i = 0; i < nums.size(); i++) {
            if (i >= interval_end) {
                interval_end = dp + 1;
                cost++;
            }

            dp = std::max(dp, nums[i] + (int)i);
        }

        return cost;
    }
};