class Solution {
private:
    std::vector<int> * m_pre;
    std::vector<int> * m_in;
    int pre_begin = 0;
    
public:
    TreeNode* buildTree(std::vector<int>& preorder, std::vector<int>& inorder) {
        m_pre = &preorder;
        m_in = &inorder;
        
        int pre_begin = 0;
        TreeNode * root = buildTree(0, preorder.size() - 1);
        return root;
    }
    
    TreeNode * buildTree(int in_begin, int in_end) {       
        if (in_begin > in_end) { return nullptr; }
        
        TreeNode * node = new TreeNode(m_pre->at(pre_begin));
        pre_begin++;
        
        if (in_begin == in_end) { return node; }
        
        int in_parent = in_begin;
        for(; m_in->at(in_parent) != node->val && in_parent < m_in->size(); in_parent++) { }
        
        node->left = buildTree(in_begin, in_parent - 1);
        node->right = buildTree(in_parent + 1, in_end);
        
        return node;
    }
};