class Solution {
public:
    Node* cloneGraph(Node* node) {
        if (node == nullptr) {
            return nullptr;
        }
        
        std::vector<Node *> node_map(101, nullptr); // array instead of hash map to make it faster (1 <= Node.val <= 100)
        Node * clone_root = nullptr;
        
        std::queue<Node *> queue;
        queue.push(node);
        
        Node * clone = new Node(node->val);
        node_map[node->val] = clone;
        
        clone_root = clone;
        
        while (!queue.empty()) { // O(V) - go through all nodes
            Node * cur_node = queue.front();
            queue.pop();
            
            clone = node_map[cur_node->val];
            
            for (Node * neighbor : cur_node->neighbors) { // O(E) - go through all neighbors of each node -> O(V + E)
                if (node_map[neighbor->val] == nullptr) {
                    Node * clone_nei = new Node(neighbor->val);
                    
                    clone->neighbors.push_back(clone_nei);
                    node_map[neighbor->val] = clone_nei;
                    
                    queue.push(neighbor);
                }
                else {
                    Node * clone_nei = node_map[neighbor->val];
                    clone->neighbors.push_back(clone_nei);
                }
            }
        }
        
        return clone_root; // Algorithm: O(V + E), Memory: O(V)
    }
};