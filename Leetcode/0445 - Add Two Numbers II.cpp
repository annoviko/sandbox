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
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        if (!l1 && !l2) { return nullptr; }
        
        std::stack<int> number1, number2;
        
        ListNode * cursor = l1;
        while(cursor) {
            number1.push(cursor->val);
            cursor = cursor->next;
        }
        
        cursor = l2;
        while(cursor) {
            number2.push(cursor->val);
            cursor = cursor->next;
        }
        
        bool carry = false;
        std::stack<int> result;
        
        while (!number1.empty() || !number2.empty()) {
            int op1 = number1.empty() ? 0 : number1.top();
            int op2 = number2.empty() ? 0 : number2.top();
            
            int value = op1 + op2 + (carry ? 1 : 0);
            carry = false;
            
            if (value > 9) {
                carry = true;
                value = value % 10;
            }
            
            if (!number1.empty())
                number1.pop();
            
            if (!number2.empty())
                number2.pop();
            
            result.push(value);
        }
        
        if (carry) {
            result.push(1);
        }
        
        ListNode * head = new ListNode(result.top());
        cursor = head;
        result.pop();
        
        while(!result.empty()) {
            cursor->next = new ListNode(result.top());
            result.pop();
            cursor = cursor->next;
        }

        return head;
    }
};