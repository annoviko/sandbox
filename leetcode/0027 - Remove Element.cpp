class Solution {
public:
    int removeElement(vector<int>& nums, int val) {
        int free_place = 0;
        for (std::size_t i = 0; i < nums.size(); i++) {
            if (val != nums[i]) {
                std::swap(nums[i], nums[free_place]);
                free_place++;
            }
        }
        
        return free_place;
    }
};