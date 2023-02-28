/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */

struct stats {
    TreeNode * node = nullptr;
    int counter = 0;
};

class Solution {
private:
    std::unordered_map<std::string, stats> m_counter;
public:
    std::vector<TreeNode*> findDuplicateSubtrees(TreeNode* root) {
        m_counter.clear();

        dfs(root);

        std::vector<TreeNode *> result;
        for (const auto & pair : m_counter) {
            if (pair.second.counter > 1) {
                result.push_back(pair.second.node);
            }
        }

        return result;
    }

private:
    std::string dfs(TreeNode * node) {
        if (node == nullptr) {
            return "";
        }

        std::string result = std::to_string(node->val) + "," + dfs(node->left) + "," + dfs(node->right);
        auto iter = m_counter.find(result);
        if (iter != m_counter.end()) {
            iter->second.counter++;
        }
        else {
            m_counter[result] = { node, 1 };
        }

        return result;
    }
};