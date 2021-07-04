class Solution {
public:
    void reorderList(ListNode* head) {
        std::vector<ListNode *> nodes; /* M = O(n) */
        ListNode * cursor = head;

        while (cursor != nullptr) {  /* T = O(n) */
            nodes.push_back(cursor);
            cursor = cursor->next;
        }

        cursor = head;
        bool lturn = true;
        for (int i = 0, j = nodes.size() - 1; i < j;) { /* T = O(n) */
            if (lturn) {
                cursor->next = nodes[j];
                cursor = cursor->next;
                i++;
            }
            else {
                cursor->next = nodes[i];
                cursor = cursor->next;
                j--;
            }

            lturn = !lturn;
        }

        cursor->next = nullptr;

        /*

        T = O(2n)
        M = O(n)
        
        */
    }
};