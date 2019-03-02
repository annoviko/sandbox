/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* left;
    Node* right;
    Node* next;

    Node() {}

    Node(int _val, Node* _left, Node* _right, Node* _next) {
        val = _val;
        left = _left;
        right = _right;
        next = _next;
    }
};
*/
class Solution {
public:
    Node* connect(Node* root) {
        if (!root) { return nullptr; }
        
        Node * head = root;
        Node * prev_level_begin = root;
        
        root->next = nullptr;
        
        while(prev_level_begin) {
            Node * prev_level_cursor = prev_level_begin;
            
            Node * cur_level_cursor = nullptr;
            Node * cur_level_begin = nullptr;
            
            while(prev_level_cursor) {
                if (prev_level_cursor->left) {
                    if (cur_level_cursor) {
                        cur_level_cursor->next = prev_level_cursor->left;
                    }
                    else {
                        cur_level_begin = prev_level_cursor->left;
                    }
                    
                    cur_level_cursor = prev_level_cursor->left;
                }
                
                if (prev_level_cursor->right) {
                    if (cur_level_cursor) {
                        cur_level_cursor->next = prev_level_cursor->right;
                    }
                    else {
                        cur_level_begin = prev_level_cursor->right;
                    }
                    
                    cur_level_cursor = prev_level_cursor->right;
                }
                
                prev_level_cursor = prev_level_cursor->next;
            }
            
            prev_level_begin = cur_level_begin;
        }
        
        return head;
    }
};