class Solution {
public:
#if defined(WITH_MEMORY)
    int findDuplicate(vector<int>& nums) {
        std::unordered_set<int> unique;
        for (auto num : nums) {
            if (unique.find(num) != unique.end()) {
                return num;
            }
            
            unique.insert(num);
        }
        
        return INT_MIN;
    }
#elif defined(WITH_SORT)
    int findDuplicate(vector<int>& nums) {
        std::sort(nums.begin(), nums.end());
        
        for (std::size_t i = 0; i < nums.size(); i++) {
            if (i + 1 != nums[i]) {
                return nums[i];
            }
        }
        
        return INT_MIN;
    }
#else
    int findDuplicate(std::vector<int>& nums) {
        int slow1 = nums[0];
        int fast = nums[0];

        do {
            slow1 = nums[slow1];
            fast = nums[nums[fast]];
            
        } while(slow1 != fast);

        int slow2 = nums[0];
        while(slow1 != slow2) {
            slow1 = nums[slow1];
            slow2 = nums[slow2];
        }
        
        return slow1;
    }  
#endif
};