class Solution {
public:
    vector<int> postorderTraversal(TreeNode* root) {
        std::vector<int> result;
        result.reserve(256);
        
        postorder(root, result);
        return result;
    }
    
private:
    void postorder(TreeNode * p_root, std::vector<int> & p_result) {
        if (p_root == nullptr) { return; }
        
        /* left -> right -> root */
        postorder(p_root->left, p_result);
        postorder(p_root->right, p_result);
        p_result.push_back(p_root->val);
    }
};