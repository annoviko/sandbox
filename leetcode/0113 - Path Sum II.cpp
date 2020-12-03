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
    std::vector<vector<int>> m_result;
public:
    vector<vector<int>> pathSum(TreeNode* root, int sum) {
        std::list<int> stack;
        traverse(root, sum, stack);
        return m_result;
    }
    
    void traverse(TreeNode * node, int sum, std::list<int> & stack) {
        if (!node) { return; }
        
        stack.push_back(node->val);
        
        sum -= node->val;
        if (sum == 0 && !node->left && !node->right) {
            std::vector<int> result;
            result.reserve(stack.size());
            for(auto iter = stack.begin(); iter != stack.end(); iter++) {
                result.push_back(*iter);
            }
            
            m_result.push_back(std::move(result));
        }
        else {
            if (node->left) {
                traverse(node->left, sum, stack);
            }
            if (node->right) {
                traverse(node->right, sum, stack);
            }
        }
        
        stack.pop_back();
    }
};