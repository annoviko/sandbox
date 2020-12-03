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
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode * head = nullptr;
        ListNode * cursor = nullptr;
        
        while(l1 && l2) {
            ListNode * next = nullptr;
            if (l1->val < l2->val) {
                next = l1;
                l1 = l1->next;
            }
            else {
                next = l2;
                l2 = l2->next;
            }
            
            if (!head) {
                head = next;
            }
            else {
                cursor->next = next;
            }
            
            cursor = next;
        }
        
        ListNode * next = l1 ? l1 : l2;
        if (head) {
            cursor->next = l1 ? l1 : l2;
        }
        else {
            head = l1 ? l1 : l2;
        }
        
        
        return head;
    }
};