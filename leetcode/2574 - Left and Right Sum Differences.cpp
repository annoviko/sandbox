class Solution {
public:
    std::vector<int> leftRightDifference(const std::vector<int>& nums) {
        std::vector<int> lv(nums.size(), 0), rv(nums.size(), 0);
        for (int i = 1; i < nums.size(); i++) {
            lv[i] += lv[i - 1] + nums[i - 1];
            rv[nums.size() - i - 1] += rv[nums.size() - i] + nums[nums.size() - i];
        }

        std::vector<int> r(nums.size(), 0);
        for (int i = 0; i < nums.size(); i++) {
            r[i] = std::abs(lv[i] - rv[i]);
        }

        return r;
    }
};