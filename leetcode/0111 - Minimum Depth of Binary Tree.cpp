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
    int minDepth(TreeNode* root) {
        if (root == nullptr) {
           return 0;
        }
        
        int level = 1;
        std::list<TreeNode*> to_consider = { root };
        
        while (true) {
          std::list<TreeNode*> next_level;
          while (!to_consider.empty()) {
             TreeNode * node = to_consider.front();
             to_consider.pop_front();
              
             if (node->left == nullptr && node->right == nullptr) {
                return level;
             }
             
             if (node->left) {
             	  next_level.push_back(node->left);
             }
             if (node->right) {
            	  next_level.push_back(node->right);
             }
          }
          
          to_consider = std::move(next_level);
          level++;
        }
        
        return -1;
    }
};