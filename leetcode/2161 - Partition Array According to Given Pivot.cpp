class Solution {
public:
    std::vector<int> pivotArray(vector<int>& nums, int pivot) {
        int smaller = 0;
        int equal = 0;
        int bigger = 0;

        for (auto v : nums) {
            if (v < pivot) {
                smaller++;
            }
            else if (v > pivot) {
                bigger++;
            }
            else {
                equal++;
            }
        }

        int low_index = 0;
        int big_index = smaller + equal;
        std::vector<int> result(nums.size(), pivot);
        for (auto v : nums) {
            if (v < pivot) {
                result[low_index] = v;
                low_index++;
            }
            else if (v > pivot) {
                result[big_index] = v;
                big_index++;
            }
        }

        return result;
    }
};