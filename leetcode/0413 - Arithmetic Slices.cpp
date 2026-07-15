#include <vector>


class Solution {
public:
    int numberOfArithmeticSlices(const std::vector<int>& nums) {
        if (nums.size() < 3) {
            return 0;
        }

        int delta = nums[1] - nums[0];
        int sequence_size = 2;
        int result = 0;

        auto count_sequence = [](const int sequence_size) -> int {
            if (sequence_size < 3) {
                return 0;
            }

            const int n = sequence_size - 2;
            const int s = (n * (n + 1)) / 2;
            return s;
        };

        for (int i = 2; i < nums.size(); i++) {
            const int cur_delta = nums[i] - nums[i - 1];
            if (cur_delta != delta) {
                result += count_sequence(sequence_size);
                sequence_size = 2;
                delta = cur_delta;
            }
            else {
                sequence_size++;
            }
        }

        result += count_sequence(sequence_size);
        return result;
    }
};
