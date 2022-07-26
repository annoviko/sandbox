class Solution {
public:
    TreeNode* sortedListToBST(ListNode* head) {
        std::vector<int> list = { };
        for (ListNode * cur = head; cur != nullptr; cur = cur->next) {
            list.push_back(cur->val);
        }
        
        return build(0, list.size() - 1, list);
    }
    
    TreeNode * build(int left, int right, const std::vector<int> & list) {
        if (left > right) {
            return nullptr;
        }
        
        int mid = (left + right) / 2;
        
        TreeNode * node = new TreeNode(list[mid]);
        
        node->left = build(left, mid - 1, list);
        node->right = build(mid + 1, right, list);
        
        return node;
    }
};