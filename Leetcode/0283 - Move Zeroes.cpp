class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        std::size_t nonzero_index = 0;
        for (std::size_t i = 0; i < nums.size(); i++) {
            if (nums[i] != 0) {
                std::swap(nums[nonzero_index], nums[i]);
                nonzero_index++;
            }
        }
    }
};