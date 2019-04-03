class Solution {
public:
    int searchInsert(std::vector<int>& nums, int target) {
        std::size_t lborder = 0;
        std::size_t rborder = nums.size() - 1;
        
        std::size_t position = 0;
        
        while (lborder <= rborder) {
            position = (rborder + lborder) / 2;
            if (nums[position] > target) {
                rborder = position - 1;
            }
            else if (nums[position] < target) {
                lborder = position + 1;
            }
            else {
                return position;
            }
        }
        
        return lborder;
    }
};