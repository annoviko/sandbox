/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
class Solution {
private:
    int getLength(ListNode * head) {
        ListNode * cursor = head;
        int result = 0;
        
        while(cursor) {
            cursor = cursor->next;
            result++;
        }
        
        return result;
    }
    
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        int lenA = getLength(headA);
        int lenB = getLength(headB);
        
        int difference = std::abs(lenA - lenB);
        if (difference > 0) {
            if (lenA > lenB) {
                for (int i = 0; i < difference; i++) {
                    headA = headA->next;
                }
            }
            else {
                for (int i = 0; i < difference; i++) {
                    headB = headB->next;
                }
            }
        }
        
        while(headA) {
            if (headA == headB) {
                return headA;
            }
            
            headA = headA->next;
            headB = headB->next;
        }
        
        return nullptr;
    }
};