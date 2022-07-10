class Solution {
public:
    bool canJump(const std::vector<int>& nums) {
        int dp = nums[0];

        for (int i = 1; (i < nums.size()) && (i <= dp); i++) {
            dp = std::max(dp, nums[i] + i);
            if (dp >= nums.size() - 1) {
                return true;
            }
        }

        return (dp >= nums.size() - 1);
    }
};