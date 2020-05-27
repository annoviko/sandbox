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
private:
    int m_max = 0;
    
public:
    int diameterOfBinaryTree(TreeNode* root) {
        if (!root) { return 0; }
        traverse(root, 0);
        return m_max;
    }
    
private:
    int traverse(TreeNode* node, int distance) {       
        int l = node->left ? traverse(node->left, distance + 1) : distance;
        int r = node->right ? traverse(node->right, distance + 1) : distance;
        
        int candidate = l + r - 2 * distance;
        m_max = std::max(candidate, m_max); // assign if the longest
        
        return l > r ? l : r; // return the longest branch
    }
};