class Solution {
public:
    std::vector<int> searchRange(const std::vector<int>& nums, int target) {
        if (nums.empty()) {
            return { -1, -1 };
        }
        std::vector<int> result;

        int left = 0;
        int right = nums.size() - 1;

        while (left < right) {
            int mid = (right + left) / 2;

            if (nums[mid] < target) {
                left = mid + 1;
            }
            else {
                right = mid;
            }
        }

        if (nums[left] != target) {
            return { -1, -1 };
        }
        result.push_back(left);

        right = nums.size() - 1;
        while (left < right) {
            int mid = (right + left) / 2 + 1;

            if (nums[mid] <= target) {
                left = mid;
            }
            else {
                right = mid - 1;
            }
        }

        result.push_back(right);
        return result;
    }
};
