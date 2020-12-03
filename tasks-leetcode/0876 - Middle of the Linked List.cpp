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
    ListNode* middleNode(ListNode* head) {
        if (!head) { return nullptr; }
        
        std::size_t length = 0;
        for (ListNode * slow = head; slow != nullptr; slow = slow->next) {
            length++;
        }
        
        std::size_t middle = length / 2;
        
        ListNode * result = head;
        
        std::size_t index = 0;
        for (; index != middle; result = result->next) {
            index++;
        }
        
        return result;
    }
};