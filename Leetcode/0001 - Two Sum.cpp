class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        std::unordered_map<int, std::size_t> addition;
        for (std::size_t i = 0; i < nums.size() - 1; i++) {
            int difference = target - nums[i];
            auto iter_addition = addition.find(difference);
            
            if (iter_addition != addition.end() && iter_addition->second != i) {
                return { i, iter_addition->second };
            }
            
            addition[nums[i]] = i; 
        }
    }
};