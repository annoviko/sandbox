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
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        if (!l1 || !l2)
            return nullptr;
        
        int appendix = 0;
        
        ListNode * result = new ListNode(0);
        ListNode * node = nullptr;
        
        while (l1 != nullptr || l2 != nullptr) {
            if (node != nullptr) {
                node->next = new ListNode(0);
                node = node->next;
            }
            else {
                node = result;
            }
            
            int value = appendix;
            
            if (l1) {
                value += l1->val;
                l1 = l1->next;
            }
            
            if (l2) {
                value += l2->val;
                l2 = l2->next;
            }
            
            appendix = value / 10;
            node->val = value % 10;
        }
        
        if (appendix > 0) {
            node->next = new ListNode(appendix);
        }
        
        return result;
    }
};