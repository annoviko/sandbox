class Solution {
public:
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        ListNode * cur_node = head;
        ListNode * left_border_node = nullptr;
        int cur_pos = 1;
        
        while ((cur_pos < left) && (cur_node != nullptr)) {
            left_border_node = cur_node;
            cur_node = cur_node->next;
            cur_pos++;
        }
    
        if (cur_node == nullptr) {
            return head;
        }
        
        ListNode * tail = cur_node;
        ListNode * begin = tail;
        
        cur_node = cur_node->next;
        cur_pos++;
        
        while ((cur_pos <= right) && (cur_node != nullptr)) {
            ListNode * new_begin = cur_node;
            cur_node = cur_node->next;
            new_begin->next = begin;

            begin = new_begin;
            cur_pos++;
        }
        
        if (left_border_node != nullptr) {
            left_border_node->next = begin;
        }
        else {
            head = begin;
        }

        tail->next = cur_node;
        
        return head; // O(N) exactly - one pass only with O(1) memory.
    }
};
