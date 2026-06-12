class Solution {
public:
    TreeNode* createBinaryTree(const std::vector<std::vector<int>>& descriptions) {
        std::unordered_map<int, TreeNode*> refs;
        std::unordered_map<int, int> parent_counter;

        for (const auto& desc : descriptions) {
            const int id_parent = desc[0];
            const int id_child = desc[1];
            const bool is_left = (desc[2] == 1);

            parent_counter[id_child]++;

            TreeNode* parent = nullptr;
            TreeNode* child = nullptr;

            auto iter = refs.find(id_parent);
            if (iter == refs.cend()) {
                parent = new TreeNode(id_parent);
                refs[id_parent] = parent;
            }
            else {
                parent = iter->second;
            }

            iter = refs.find(id_child);
            if (iter == refs.cend()) {
                child = new TreeNode(id_child);
                refs[id_child] = child;
            }
            else {
                child = iter->second;
            }

            if (is_left) {
                parent->left = child;
            }
            else {
                parent->right = child;
            }
        }

        for (const auto& desc : descriptions) {
            const int node_id = desc[0];
            if (parent_counter[node_id] == 0) {
                return refs[node_id];
            }
        }

        return nullptr;
    }
};