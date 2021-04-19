class Solution {
public:
    TreeNode* sortedArrayToBST(const std::vector<int>& nums) {
        return build(nums, 0, nums.size());
    }

private:
    TreeNode* build(const std::vector<int> & nums, const std::size_t lborder, const std::size_t rborder) {
        if (lborder >= rborder || lborder > nums.size() /* std::size_t overflow */ || rborder > nums.size()) {
            return nullptr;
        }

        std::size_t index = lborder + (rborder - lborder) / 2;

        TreeNode * node = new TreeNode(nums[index]);
        node->left = build(nums, lborder, index);
        node->right = build(nums, index + 1, rborder);

        return node;
    }
};