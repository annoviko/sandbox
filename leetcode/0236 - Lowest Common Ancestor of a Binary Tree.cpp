class Solution {
public:
#if 0
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        std::list<TreeNode *> first_stack;
        std::list<TreeNode *> second_stack;

        find_node(root, p, first_stack);    /* O(n) */
        find_node(root, q, second_stack);   /* O(n) */

        TreeNode * result = nullptr;

        auto iter1 = first_stack.begin();
        auto iter2 = second_stack.begin();

        while (iter1 != first_stack.end() && iter2 != second_stack.end() && *iter1 == *iter2) {  /* O(n) */
            result = *iter1;

            iter1++;
            iter2++;
        }


        /* Total algorithmic complexity: 3 * O(n) = O(n) */
        /* Total memory complexity: 3 * O(n) = O(n) */
        return result;
    }

private:
    bool find_node(TreeNode * p_node, TreeNode * p_target, std::list<TreeNode *> & p_stack) {
        if (p_node == nullptr) {
            return false;
        }

        if (p_node == p_target) {
            p_stack.push_front(p_node);
            return true;
        }

        bool is_found = find_node(p_node->left, p_target, p_stack);
        if (!is_found) {
            is_found = find_node(p_node->right, p_target, p_stack);
        }

        if (is_found) {
            p_stack.push_front(p_node);
        }

        return is_found;
    }
#else
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if ((root == p) || (root == q) || (root == nullptr)) {
            return root;
        }

        TreeNode * parent1 = lowestCommonAncestor(root->left, p, q);
        TreeNode * parent2 = lowestCommonAncestor(root->right, p, q);

        if (parent1 && parent2) {
            return root;    /* if both branches has targets, then this is common */
        }
        else {
            return parent1 ? parent1 : parent2;  /* if only one branch has target, then return its common node */
        }
    }
#endif
};
