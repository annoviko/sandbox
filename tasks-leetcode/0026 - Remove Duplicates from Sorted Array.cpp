class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        if (nums.empty()) { return 0; }
        
        std::size_t free_index = 0;
        int prev_element = nums[0];
        
        for (std::size_t i = 1; i < nums.size(); i++) {
            if (prev_element != nums[i]) {
                prev_element = nums[i];
                
                free_index++;
                std::swap(nums[free_index], nums[i]);
            }
        }
        
        return free_index + 1;
    }
};