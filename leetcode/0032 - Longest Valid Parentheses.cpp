class Solution {
public:
    int longestValidParentheses(const std::string& s) {
        std::stack<std::size_t> unmerged_brackets;
        for (std::size_t i = 0; i < s.size(); i++) {
            if (unmerged_brackets.empty()) {
                unmerged_brackets.push(i);
            }
            else if ((s[i] == ')') && (s[unmerged_brackets.top()] == '(')) {
                unmerged_brackets.pop();
            }
            else {
                unmerged_brackets.push(i);
            }
        }

        std::size_t max_length = 0;
        std::size_t right = s.size();

        while(!unmerged_brackets.empty()) {
            const std::size_t left = unmerged_brackets.top();
            unmerged_brackets.pop();

            max_length = std::max(max_length, right - left - 1);
            right = left;
        }

        max_length = std::max(max_length, right);
        return static_cast<int>(max_length);
    }
};