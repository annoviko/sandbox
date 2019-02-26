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
    bool isSameTree(TreeNode* p, TreeNode* q) {
        return traverse(p, q);
    }
    
    bool traverse(TreeNode * p, TreeNode * q) {
        bool result = true;
        
        if (!p && !q) {
            return true;
        }
        
        if ( (!!p != !!q) ||
             (p->val != q->val) || 
             (!!p->left != !!q->left) ||
             (!!p->right != !!q->right) )
        {
            return false;
        }
        
        if (p->left) {
            result = traverse(p->left, q->left);
        } 

        if (result && p->right) {
            result = traverse(p->right, q->right);
        }
        
        return result;
    }
};