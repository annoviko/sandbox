class Solution {
    std::vector<int> stack;
    std::vector<std::vector<int>> result;

    int value_limit;
    int amount_limit;

public:
    std::vector<std::vector<int>> combine(int n, int k) {
        value_limit = n;
        amount_limit = k;

        backtracking(1);
        return result;
    }

private:
    void backtracking(int value) {
        if (stack.size() < amount_limit) {
            for (int i = value; i <= value_limit; i++) {
                stack.push_back(i);
                backtracking(i + 1);
                stack.pop_back();
            }
        }
        else if (stack.size() == amount_limit) {
            result.push_back(stack);
        }
    }
};