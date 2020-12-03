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
    ListNode* reverseList(ListNode* head) {
        ListNode* previous = nullptr;
        ListNode* current = head;
        
        while(current) {
            ListNode* next = current->next;
            current->next = previous;
            
            previous = current;
            current = next;
        }
        
        return previous;
    }
};