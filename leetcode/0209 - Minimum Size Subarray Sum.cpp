#include <vector>

class Solution {
public:
    int minSubArrayLen(int target, const std::vector<int>& nums) {
        int left = 0;

        int sum = 0;
        int min_length = INT_MAX;
        int cur_length = 0;

        for (int i = 0; i < nums.size(); i++) {
            sum += nums[i];
            cur_length++;

            while (sum >= target) {
                min_length = std::min(min_length, cur_length);

                sum -= nums[left];
                cur_length--;
                left++;
            }
        }

        return (min_length == INT_MAX) ? 0 : min_length;
    }
};