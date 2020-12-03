#include <vector>
#include <list>


struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};
 
class Solution {
public:
#if defined(NO_RECURSION)
    std::vector<int> rightSideView(TreeNode* root) {
        if (!root) { return { }; }

        std::vector<int> result;

        std::list<TreeNode*> current_level;
        std::list<TreeNode*> next_level = { root };

        while (!next_level.empty()) {
            result.push_back(next_level.back()->val);
            current_level = std::move(next_level);

            for (auto& node : current_level) {
                if (node->left) {
                    next_level.push_back(node->left);
                }

                if (node->right) {
                    next_level.push_back(node->right);
                }
            }
        }

        return result;
    }
#else
    std::vector<int> rightSideView(TreeNode* root) {
        if (!root) { return { }; }
        std::vector<int> result;
        recursion(root, 1, result);
        return result;
    }

    void recursion(TreeNode* node, int level, std::vector<int> & result) {
        if (!node) { return; }
        if (level > result.size()) { result.push_back(node->val); }

        recursion(node->right, level + 1, result);
        recursion(node->left, level + 1, result);
    }
#endif
};