class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        if (nums.empty() || nums.size() < k || k == 0) { return -1; }
        
        std::sort(nums.begin(), nums.end(), std::greater<int>());
        return nums[k - 1];
    }
};