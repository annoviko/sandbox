class Solution {
public:
    bool isValidBST(TreeNode* root) {
        if (!root) {
            return true;
        }
        
        return traverseAndCheckNode(root, std::nullopt_t, std::nullopt_t);
    }
    
    bool traverseAndCheckNode(TreeNode * node, std::optional<int> lower, std::optional<int> upper) {
        if (upper && (node->val >= upper)) {
            return false;
        }
        
        if (lower && (node->val <= lower)) {
            return false;
        }
        
        bool result = true;
        if (node->left) {
            result = traverseAndCheckNode(node->left, lower, node->val);
        }
        
        if (result && node->right) {
            result = traverseAndCheckNode(node->right, node->val, upper);
        }
        
        return result;
    }
};
