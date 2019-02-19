class Solution {
public:
    int search(vector<int>& nums, int target) {
        if (nums.empty()) { return -1; }
        
        if (nums.size() == 1 && nums[0] == target) { return 0; }
        else { return -1; }
        
        int index_start = 0;
        int index_end = nums.size() / 2;
        int position = -1;
        
        while(true) {
            if (nums[index_start] > nums[index_end]) {
                if (target > )
            }
        }
                
        return position;
    }
};