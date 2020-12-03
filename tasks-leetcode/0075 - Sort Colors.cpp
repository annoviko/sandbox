class Solution {
public:
    void sortColors(std::vector<int>& nums) {
        onePassSort(nums);
    }

    void onePassSort(std::vector<int>& nums) {
        std::size_t idx0 = 0;                    // idx0++
        std::size_t idx2 = nums.size();          // --idx2

        for (long long i = 0; i < idx2; i++) {
            if (nums[i] == 0) {
                std::swap(nums[i], nums[idx0++]);
            }
            else if (nums[i] == 2) {
                std::swap(nums[i--], nums[--idx2]);
            }
        }
    }

    void countSort(std::vector<int>& nums) {
        std::vector<int> counter(3);
        for (auto color : nums) {
            counter[color]++;
        }

        std::size_t cursor = 0;
        for (std::size_t i = 0; i < counter.size(); i++) {
            while (counter[i] > 0) {
                nums[cursor++] = i;
                counter[i]--;
            }
        }
    }
};