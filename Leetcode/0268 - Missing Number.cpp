class Solution {
public:
    int missingNumber(vector<int>& nums) {
        int sum = 0;
        int expected = 0;
        for (int i = 0; i < nums.size(); i++) {
            sum += nums[i];
            expected += i;
        }
        
        expected += nums.size();
        return expected - sum;
    }
};