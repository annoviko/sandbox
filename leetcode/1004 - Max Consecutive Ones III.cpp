#include <algorithm>
#include <vector>


class Solution {
public:
    int longestOnes(const std::vector<int>& nums, int k) {
        std::size_t begin = 0;
        std::size_t end = 0;

        std::size_t zero_counter = 0;
        for (; end < nums.size() && zero_counter != k; end++) {	/* O(n) */
            if (nums[end] == 0) {
                zero_counter++;
            }
        }

        std::size_t result = end - begin;
        while (end < nums.size()) {	/* 2 * O(n) */
            if (nums[end] == 0) {
                zero_counter++;
                if (zero_counter > k) {
                    while (zero_counter != k) {
                        if (nums[begin] == 0) {
                            zero_counter--;
                        }

                        begin++;
                    }
                }
            }

            result = std::max(result, end - begin + 1);
            end++;
        }

        return result;
    }
};
