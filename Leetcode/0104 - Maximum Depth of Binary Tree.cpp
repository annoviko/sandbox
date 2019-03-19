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
private:
    int m_max = 0;
    
public:
    int maxDepth(TreeNode* root) {
        m_max = 0;
        if (root) {
            traverse(root, 1);
        }  
        
        return m_max;
    }
    
    void traverse(TreeNode * node, int level) {       
        if (level > m_max) {
            m_max = level;
        }
        
        if (node->left) {
            traverse(node->left, level + 1);
        }
        
        if (node->right) {
            traverse(node->right, level + 1);
        }
    }
};