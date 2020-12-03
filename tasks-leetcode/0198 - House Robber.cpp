#include <vector>
#include <iostream>
#include <algorithm>

class Solution {
public:
    int rob(const std::vector<int>& nums) {
        if (nums.empty()) { return 0; }

        std::vector<int> max_val(nums.size(), 0);
        max_val[0] = nums[0];

        for (std::size_t i = 1; i < nums.size(); i++) {
            if (i < 2) {
                max_val[i] = std::max(max_val[i - 1], nums[i]);
            }
            else {
                max_val[i] = std::max(max_val[i - 1], nums[i] + max_val[i - 2]);
            }
        }

        return max_val.back();
    }
};