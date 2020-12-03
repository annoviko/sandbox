class Solution {
public:
    int findLeftBorder(std::vector<int> & nums, int target) {
        int begin = 0, end = nums.size();
        while (begin != end) {
            int middle = (end + begin) / 2;
            int value = nums[middle];

            if (value >= target) {
                end = middle;
            }
            else {
                begin = middle + 1;
            }
        }

        return begin;
    }

    int findRightBorder(std::vector<int> & nums, int target) {
        int begin = 0, end = nums.size();
        while (begin != end) {
            int middle = (end + begin) / 2;
            int value = nums[middle];

            if (value > target) {
                end = middle;
            }
            else {
                begin = middle + 1;
            }
        }

        return begin;
    }

    std::vector<int> searchRange(std::vector<int>& nums, int target) {
        int lborder = findLeftBorder(nums, target);
        if (lborder >= nums.size() || nums[lborder] != target) {
            //std::cout << "Not found" << std::endl;
            return { -1, -1 };
        }

        int rborder = findRightBorder(nums, target) - 1;

        //std::cout << lborder << " (" << nums[lborder] << "), " << rborder << " (" << nums[rborder] << ")" << std::endl;
        return { lborder, rborder };
    }
};