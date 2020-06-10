class Solution {
public:
    ListNode* swapPairs(ListNode* head) {
        ListNode * cur = head;
        ListNode * prev = nullptr;
        
        while(cur) {
            if (cur->next) {
                ListNode * next = cur->next;
                cur->next = next->next;
                next->next = cur;
                
                if (prev) {
                    prev->next = next;
                }
                
                if (cur == head) {
                    head = next;
                }
            }
            
            prev = cur;
            cur = cur->next;
        }
        
        return head;
    }
};