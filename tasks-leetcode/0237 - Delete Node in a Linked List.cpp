/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
public:
    void deleteNode(ListNode* node) {
        if (node && node->next) {
            ListNode * next = node->next;
            
            node->next = next->next;    /* previous became current */
            node->val = next->val;
            
            delete next;
        }
    }
};