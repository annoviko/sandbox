// both are optimal

#if defined(OPTIMAL_1)
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        return find(root, p, q);
    }
    
    TreeNode * find(TreeNode * node, TreeNode * p, TreeNode * q) {
        if (node == nullptr) {
            return nullptr;
        }
        
        if ((node == p) || (node == q)) {
            return node;
        }
        
        TreeNode * result1 = find(node->left, p, q);
        TreeNode * result2 = find(node->right, p, q);
        
        if (result1 != nullptr && result2 != nullptr) {
            return node;
        }
        
        return result1 != nullptr ? result1 : result2;
    }
};
#else // use binary search tree property
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if ((root->val < p->val) && (root->val < q->val)) {
            return lowestCommonAncestor(root->right, p, q);
        }
        else if ((root->val > p->val) && (root->val > q->val)) {
            return lowestCommonAncestor(root->left, p, q);
        }
        
        return root;
    }
};
#endif