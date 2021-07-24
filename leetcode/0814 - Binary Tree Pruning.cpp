/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    TreeNode* pruneTree(TreeNode* root) {
        const bool to_clean = clean(root);
        if (to_clean) {
            root = nullptr;
        }
        
        return root;
    }
    
private:
    bool clean(TreeNode * node) {
        if (!node) {
            return true;
        }
        
        const bool to_clean_left = clean(node->left);
        if (node->left && to_clean_left) {
            delete node->left;
            node->left = nullptr;
        }
        
        const bool to_clean_right = clean(node->right);
        if (node->right && to_clean_right) {
            delete node->right;
            node->right = nullptr;
        }
        
        if (to_clean_left && to_clean_right && node->val == 0) {
            return true;
        }
        
        return false;
    }
};