class Solution {
public:
    int maxSubArray(std::vector<int>& nums) {
        if (nums.empty()) { return 0; }
        
        int previous = nums[0];
        int result = previous;
        
        for (std::size_t i = 1; i < nums.size(); i++) {
            int current = nums[i];
            
            // check if result can be better
            if (previous > 0) {
                current += previous;
            }
            
            // update if much better result is appeared
            if (current > result) {
                result = current;
            }
            
            previous = current;
        }
        
        return result;
    }
};