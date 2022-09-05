class Solution {
public:
    bool hasPathSum(TreeNode* root, int targetSum) {
        if (!root) {
            return false;
        }
        
        targetSum -= root->val;
        
        if ((targetSum == 0) && (root->left == nullptr) && (root->right == nullptr)) {
            return true;
        }
        
        return hasPathSum(root->left, targetSum) || hasPathSum(root->right, targetSum);
    }
};