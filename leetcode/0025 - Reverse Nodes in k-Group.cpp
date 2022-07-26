class Solution {
public:
    ListNode* reverseKGroup(ListNode* head, int k) {
        ListNode * new_head = nullptr;
        ListNode * prev_tail = nullptr;

        ListNode * next_head = head;
        ListNode * next_tail = nullptr;

        // O(N)
        while(reverse_group(next_head, k, next_tail)) {
            if (prev_tail) {
                prev_tail->next = next_head;
            }

            if (!new_head) {
                new_head = next_head;
            }

            prev_tail = next_tail;
            next_head = next_tail->next;
        }

        return new_head ? new_head : head;
    }

    bool reverse_group(ListNode *& head, int k, ListNode *& tail) {
        if (!head) {
            return false;
        }

        int counter = 0;
        ListNode * cur = head;

        for (; cur != nullptr && counter < k; counter++, cur = cur->next) { }

        if (counter != k) {
            return false;
        }

        counter = 0;

        ListNode * prev = nullptr;
        cur = head;
        for (; cur != nullptr && counter < k; counter++) {
            ListNode * next_cur = cur->next;
            cur->next = prev;
            prev = cur;
            cur = next_cur;
        }

        tail = head;
        tail->next = cur;
        head = prev;
        return true;
    }
};
