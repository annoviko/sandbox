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
    bool isUnivalTree(TreeNode* root) {
        return isUnivalTree(root, root->val);
    }
    
    bool isUnivalTree(TreeNode * root, int value) {
        if (!root) {
            return true;
        }
        
        if (root->val != value) {
            return false;
        }
        
        return isUnivalTree(root->left, value) && isUnivalTree(root->right, value);
    }
};