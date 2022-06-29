class Solution {
public:
    ListNode* deleteDuplicates(ListNode* head) {
        if (head == nullptr) {
            return head;
        }
        
        ListNode * prev = head;

        for (auto cur = prev->next; cur != nullptr; cur = cur->next) {
            if (prev->val != cur->val) {
                prev = cur;
            }
            else {
                prev->next = cur->next;
            }
        }

        return head;
    }
};