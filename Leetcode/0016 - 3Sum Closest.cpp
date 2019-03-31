class Solution {
public:
    int threeSumClosest(vector<int>& nums, int target) {
        if (nums.size() < 3) {
            return 0;
        }
        
        std::sort(nums.begin(), nums.end());

        int result = nums[0] + nums[1] + nums[2];
        for (int i = 0; i < nums.size() - 1; i++) {
            int cur = nums[i];
            int lborder = i + 1;
            int rborder = nums.size() - 1;
            
            while (lborder < rborder) {
                int sum = cur + nums[lborder] + nums[rborder];
                if (sum == target) {
                    return sum;
                }
                else if (sum > target) {
                    rborder--;
                    while((rborder > lborder) && (nums[rborder] == nums[rborder + 1])) {
                        rborder--;
                    }
                }
                else {
                    lborder++;
                    while((rborder > lborder) && (nums[lborder] == nums[lborder - 1])) {
                        lborder++;
                    }
                }
                
                if (std::abs(target - result) > std::abs(target - sum)) {
                    result = sum;
                }
            }
        }
        
        return result;
    }
};