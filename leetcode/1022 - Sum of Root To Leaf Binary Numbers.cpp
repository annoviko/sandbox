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
    int sumRootToLeaf(TreeNode* root) {
        return dfs(root, 0);
    }

private:
    int dfs(TreeNode* cur, int value) {
        if (cur == nullptr) {
            return 0;
        }

        value = (value << 1) + cur->val;
        if (cur->left == nullptr && cur->right == nullptr) {
            return value;   /* leaf */
        }

        int sum = 0;
        sum += dfs(cur->left, value);
        sum += dfs(cur->right, value);

        value = (value >> 1);
        return sum;
    }
};