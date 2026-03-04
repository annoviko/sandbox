#include <numeric>
#include <vector>

class Solution {
public:
    void rotate(std::vector<int>& nums, int k) {
        if (k == 0) {
            return;
        }

        int collisions = std::gcd(nums.size(), k) - 1;

        int pos = 0;
        int val = nums[pos];
        int anchor = 0;

        for (int i = 0; i < nums.size(); i++) {
            pos = (pos + k) % nums.size();

            int next_val = nums[pos];
            nums[pos] = val;

            if ((pos == anchor) && (collisions > 0)) {
                pos = (pos + 1) % nums.size();
                collisions--;

                anchor = pos;
                next_val = nums[pos];
            }

            val = next_val;
        }
    }
};
