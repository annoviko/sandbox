class Solution {
public:
    std::vector<std::vector<int>> zigzagLevelOrder(TreeNode* root) {
        if (!root) { return { }; }
        
        std::vector<std::vector<int>> result;
        
        std::list<TreeNode *> level;
        level.push_back(root);
        
        traverse(level, true, result);
        return result;
    }
    
private:
#if defined(LONG_VERSION)
    void traverse(std::list<TreeNode *> & prev_level, bool forward, std::vector<std::vector<int>> & result) {
        std::list<TreeNode *> level;
        
        std::vector<int> level_values;
        level_values.reserve(prev_level.size() * 2);
        
        if (forward) {
            for (auto node : prev_level) {
                level_values.push_back(node->val);
                
                if (node->left) {
                    level.push_back(node->left);
                }
                
                if (node->right) {
                    level.push_back(node->right);
                }
            }
        }
        else {
            for (auto riter = prev_level.rbegin(); riter != prev_level.rend(); riter++) {
                level_values.push_back((*riter)->val);
                
                if ((*riter)->right) {
                    level.push_front((*riter)->right);
                }

                if ((*riter)->left) {
                    level.push_front((*riter)->left);
                }
            }
        }
        
        result.push_back(std::move(level_values));

        if (!level.empty()) {
            prev_level = std::move(level);
            traverse(prev_level, !forward, result);
        }
    }
#else
    void traverse(std::list<TreeNode *> & prev_level, bool forward, std::vector<std::vector<int>> & result) {
        std::list<TreeNode *> level;

        std::vector<int> level_values;
        level_values.reserve(prev_level.size() * 2);

        for (auto node : prev_level) {
            if (node->left) {
                level.push_back(node->left);
            }

            if (node->right) {
                level.push_back(node->right);
            }

            level_values.push_back(node->val);
        }

        if (!forward) {
            std::reverse(level_values.begin(), level_values.end());
        }

        result.push_back(std::move(level_values));

        if (!level.empty()) {
            prev_level = std::move(level);
            traverse(prev_level, !forward, result);
        }
    }
#endif
};