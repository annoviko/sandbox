/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* next;
    Node* random;

    Node() {}

    Node(int _val, Node* _next, Node* _random) {
        val = _val;
        next = _next;
        random = _random;
    }
};
*/

#define WITHOUT_MEMORY

class Solution {
public:
#ifndef WITHOUT_MEMORY
    Node* copyRandomList(Node* head) {
        if (!head) { return nullptr; }
        
        std::unordered_map<Node *, Node *> matching_map;
        for (Node * node = head; node != nullptr; node = node->next) {
            Node * dummy = new Node(0, nullptr, nullptr);
            matching_map[node] = dummy;
        }
        
        Node * copy_head = matching_map[head];
        
        while (head != nullptr) {
            Node * cursor = matching_map[head];
            
            cursor->val = head->val;
            
            if (head->next) {
                cursor->next = matching_map[head->next];
            }
            
            if (head->random) {
                cursor->random = matching_map[head->random];
            }
            
            head = head->next;
        }
        
        return copy_head;
    }
#else
    Node* copyRandomList(Node* head) {
        if (!head) { return nullptr; }
        
        /* create nodes and store in them corresponding */
        Node * cursor = head;
        while (cursor != nullptr) {
            Node * next = cursor->next;
            
            Node * copy = new Node(0, nullptr, nullptr);
            copy->val = cursor->val;
            copy->next = next;      // copy -> next
            cursor->next = copy;    // cursor -> copy -> next
            
            cursor = next;
        }
        
        /* assign random nodes */
        cursor = head;
        while (cursor != nullptr) {
            if (cursor->random) {
                Node * copy = cursor->next;
                copy->random = cursor->random->next; // random = cursor -> original element -> copy
            }
            
            cursor = cursor->next->next; // cursor -> copy -> next
        }
        
        /* restore original list and set next nodes */
        cursor = head;
        Node * copy_head = head->next;
        while (cursor != nullptr) {
            Node * copy = cursor->next;
            Node * original_next = copy->next;
            Node * copy_next = original_next ? original_next->next : nullptr;
            
            copy->next = copy_next;
            cursor->next = original_next;
            
            cursor = cursor->next;
        }
        
        return copy_head;
    }
#endif
};