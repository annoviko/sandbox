#include <iostream>

class Solution {
public:
    int maxSubArray(const std::vector<int> & nums) {
        int maximum = nums[0];
        int current = nums[0];

        for (int i = 1; i < nums.size(); i++) {
            const int value = nums[i];
            current += value;
            if (current < value) {
                current = value;        // sum already less than current value, then reset it
            }

            if (current > maximum) {
                maximum = current;
            }
        }

        return maximum;
    }
};
