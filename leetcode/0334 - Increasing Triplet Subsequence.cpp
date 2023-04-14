class Solution {
public:
    bool increasingTriplet(std::vector<int>& nums) {
        int seq_one_elem = INT_MAX;
        int seq_two_elems = INT_MAX;

        for (int i = 0; i < nums.size(); i++) {
            if (nums[i] <= seq_one_elem) {
                seq_one_elem = nums[i];
            }
            else if (nums[i] <= seq_two_elems) {
                seq_two_elems = nums[i];
            }
            else {
                return true;
            }
        }

        return false;
    }
};