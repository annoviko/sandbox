/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    bool isSymmetric(TreeNode* root) {
        if (!root) { return true; }
        if (!!root->left != !!root->right) { return false; }
        
        if (root->left) {
            return isSymmetric(root->left, root->right);
        }
        
        return true;
    }
    
    bool isSymmetric(TreeNode * node1, TreeNode * node2) {
        if (node1->val != node2->val || 
            !!node1->left != !!node2->right || 
            !!node1->right != !!node2->left) 
        {
            return false;
        }
        
        bool result = true;
        if (node1->left) {
            result = isSymmetric(node1->left, node2->right);
        }
        
        if (result && node1->right) {
            result = isSymmetric(node1->right, node2->left);
        }
        
        return result;
    }
};