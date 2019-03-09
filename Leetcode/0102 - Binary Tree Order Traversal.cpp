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
    std::vector<std::vector<int>> levelOrder(TreeNode* root) {
        if (!root) { return { }; }
        
        std::list<TreeNode *> level;
        level.push_back(root);
        
        std::vector<std::vector<int>> result = { { root->val } };
        
        traverse(level, result);
        return result;
    }
    
private:
    void traverse(std::list<TreeNode *> & prev_level, std::vector<std::vector<int>> & result) {
        if (prev_level.empty()) { return; }
        
        std::list<TreeNode *> level;
        
        std::vector<int> level_values = { };
        level_values.reserve(prev_level.size() * 2);
        
        for (const auto node : prev_level) {
            if (node->left) {
                level.push_back(node->left);
                level_values.push_back(node->left->val);
            }
            if (node->right) {
                level.push_back(node->right);
                level_values.push_back(node->right->val);
            }
        }
        
        if (!level.empty()) {
            prev_level = std::move(level);
            result.push_back(std::move(level_values));
            traverse(prev_level, result);
        }
    }
};