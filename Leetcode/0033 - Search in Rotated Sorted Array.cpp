class Solution {
public:
    int search(std::vector<int>& nums, int target) {
        if (nums.empty()) { return -1; }
        if (nums.size() == 1) {
            return nums[0] == target ? 0 : -1;
        }
        
        int lborder = 0;
        int rborder = nums.size() - 1;
        int middle = (rborder + lborder) / 2;

        do {
            if (target == nums[lborder]) { return lborder; }
            if (target == nums[rborder]) { return rborder; }
            if (target == nums[middle]) { return middle; }

            if (target > nums[lborder]) {
                if (target > nums[middle] && nums[middle] > nums[lborder]) {
                    lborder = middle;
                }
                else {
                    rborder = middle;
                }
            }
            else if (target < nums[rborder]) {
                if (target < nums[middle] && nums[middle] < nums[rborder]) {
                    rborder = middle;
                }
                else {
                    lborder = middle;
                }
            }
            else {
                return -1;
            }

            middle = (rborder + lborder) / 2;
        } while (lborder != rborder - 1 && lborder != rborder);

        return -1;
    }
};