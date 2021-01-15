#include <algorithm>
#include <vector>
#include <unordered_map>


#if defined(TREE_SOLUTION)  /* DFS is not able to pass time test - too slow */
class Solution {
public:
    int minOperations(const std::vector<int>& nums, int x) {
        int result = traverse(nums, x, 0, nums.size() - 1);
        return result != INT_MAX ? result : -1;
    }

private:
    int traverse(const std::vector<int>& nums, int x, int lborder, int rborder) {
        int left_result = INT_MAX;
        int right_result = INT_MAX;

        if (lborder > rborder) {
            return INT_MAX;
        }

        if (lborder < nums.size()) {
            const int delta = x - nums[lborder];
            if (delta > 0) {
                int sub_left = traverse(nums, delta, lborder + 1, rborder);
                if (sub_left != INT_MAX) {
                    left_result = 1 + sub_left;
                }
            }
            else if (x - nums[lborder] == 0) {
                return 1;
            }
        }

        if (rborder >= 0) {
            const int delta = x - nums[rborder];
            if (delta > 0) {
                int sub_right = traverse(nums, delta, lborder, rborder - 1);
                if (sub_right != INT_MAX) {
                    right_result = 1 + sub_right;
                }
            }
            else if (x - nums[rborder] == 0) {
                return 1;
            }
        }

        return left_result > right_result ? right_result : left_result;
    }
};
#else
class Solution {
public:
    int minOperations(const std::vector<int>& nums, int x) {
        int total_sum = 0;

        std::unordered_map<int, int> prefix_sum;
        prefix_sum[0] = -1;

        for (int i = 0; i < nums.size(); i++) {
            total_sum += nums[i];
            prefix_sum[total_sum] = i;
        }

        int target = total_sum - x;
        if (target < 0) { return -1; }

        int longest_sub_array = -1;

        int current_sum = 0;
        for (int i = 0; i < nums.size(); i++) {
            current_sum += nums[i];

            int required_prefix_sum = current_sum - target;
            auto iter_prefix_sum = prefix_sum.find(required_prefix_sum);
            if (iter_prefix_sum != prefix_sum.end()) {
                const int sub_array_size = i - iter_prefix_sum->second;
                longest_sub_array = std::max(sub_array_size, longest_sub_array);
            }
        }

        if (longest_sub_array >= 0) {
            return nums.size() - longest_sub_array;
        }

        return -1;
    }
};
#endif