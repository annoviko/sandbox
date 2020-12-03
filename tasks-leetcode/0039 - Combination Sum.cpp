class Solution {
    std::vector<int> to_process;
    std::vector<int> stack;
    std::vector<std::vector<int>> result;

public:
    std::vector<std::vector<int>> combinationSum(const std::vector<int>& candidates, int target) {
        if (candidates.empty()) {
            return { };
        }

        to_process = candidates;
        std::sort(to_process.begin(), to_process.end());

        handle_tree(to_process.begin(), target);

        return result;
    }

private:
    void handle_tree(std::vector<int>::iterator cur, int target) {
        if (target > 0) {
            /* explore with the same step and others if they are less than rest of the target */
            for (; (cur != std::end(to_process)) && (*cur <= target); cur++) {
                stack.push_back(*cur);
                handle_tree(cur, target - *cur);
            }
        }
        else if (target == 0) {
            /* result is found */
            result.push_back(stack);
        }

        /* to big - just left the method */

        if (!stack.empty()) {
            stack.pop_back();
        }
    }
};