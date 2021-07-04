class Solution {
public:
    std::vector<int> preorderTraversal(TreeNode* root) {
        std::vector<int> result;
        result.reserve(512);

        preorder(root, result);
        return result;
    }

private:
    void preorder(TreeNode * p_root, std::vector<int> & p_result) {
        /* root -> left -> right */

        if (p_root == nullptr) {
            return;
        }

        p_result.push_back(p_root->val);
        preorder(p_root->left, p_result);
        preorder(p_root->right, p_result);
    }
};
