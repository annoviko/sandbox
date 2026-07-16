#include <algorithm>
#include <numeric>
#include <vector>


class Solution {
public:
    long long gcdSum(const std::vector<int>& nums) {
        std::vector<int> gcd_grid(nums.size(), 0);

        int cur_max = 0;
        for (int i = 0; i < nums.size(); i++) {
            cur_max = std::max(cur_max, nums[i]);
            gcd_grid[i] = std::gcd(cur_max, nums[i]);
        }

        std::sort(gcd_grid.begin(), gcd_grid.end());
        int l = 0, r = gcd_grid.size() - 1;
        std::int64_t sum = 0;

        while (l < r) {
            sum += std::gcd(gcd_grid[l], gcd_grid[r]);
            l++;
            r--;
        }

        return sum;
    }
};