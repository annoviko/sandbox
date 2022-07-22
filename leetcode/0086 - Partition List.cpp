class Solution {
public:
#if 0
    // Complexity: O(N), Memory: O(1), One Pass, Move small node to the correct position
    ListNode* partition(ListNode* head, int x) {
        if (head == nullptr) {
            return nullptr;
        }

        ListNode * left_border = nullptr;
        ListNode * prev = nullptr;
        ListNode * cursor = head;

        while (cursor != nullptr) {
            if (cursor->val < x) {
                if (prev == left_border) {
                    prev = cursor;
                    left_border = cursor;
                    cursor = cursor->next;

                    continue;
                }

                ListNode * node_to_move = cursor;

                prev->next = cursor->next;
                cursor = cursor->next;

                if (left_border != nullptr) {
                    node_to_move->next = left_border->next;
                    left_border->next = node_to_move;

                    left_border = node_to_move;
                }
                else {
                    node_to_move->next = head;
                    left_border = node_to_move;
                    head = node_to_move;
                }
            }
            else {
                prev = cursor;
                cursor = cursor->next;
            }
        }
        
        return head;
    }
#else
    // Complexity: O(N), Memory: O(1), One Pass, Move node into small and big list and then connect them.
    ListNode* partition(ListNode* head, int x) {
        ListNode *small_head = nullptr, *small_cur = nullptr;
        ListNode *big_head = nullptr, *big_cur = nullptr;

        for (; head != nullptr; head = head->next) {
            if (head->val < x) {
                if (small_cur != nullptr) {
                    small_cur->next = head;
                }
                else {
                    small_head = head;
                }

                small_cur = head;
            }
            else {
                if (big_cur != nullptr) {
                    big_cur->next = head;
                }
                else {
                    big_head = head;
                }

                big_cur = head;
            }
        }

        if (small_cur != nullptr) {
            small_cur->next = big_head;
            head = small_head;
        }
        else {
            head = big_head;
        }

        if (big_cur != nullptr) {
            big_cur->next = nullptr;
        }

        return head;
    }
#endif
};
