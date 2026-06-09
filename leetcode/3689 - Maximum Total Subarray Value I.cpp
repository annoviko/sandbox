class Solution {
public:
    long long maxTotalValue(vector<int>& nums, int k) {
        long long max = 0, min = INT_MAX;
        for (int i = 0; i < nums.size(); i++) {
            max = std::max((long long) nums[i], max);
            min = std::min((long long) nums[i], min);
        }

        return (max - min) * k;
    }
};