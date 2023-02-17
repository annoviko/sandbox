class Solution {
    TreeNode * prev_value = nullptr;
    int delta = INT_MAX;

public:
    int minDiffInBST(TreeNode* root) {
        prev_value = nullptr;
        delta = INT_MAX;

        dfs(root);

        return delta;
    }

private:
    void update_delta(TreeNode * node) {
        if (prev_value != nullptr) {
            delta = std::min(delta, node->val - prev_value->val);
        }

        prev_value = node;
    }

    void dfs(TreeNode * node) {
        if (node->left == nullptr) {
            update_delta(node);
        }
        else {
            dfs(node->left);
            update_delta(node);
        }

        if (node->right != nullptr) {
            dfs(node->right);
        }
    }
};