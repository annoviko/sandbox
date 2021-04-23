#if defined(COMPLEXITY_THE_SAME_BUT_BAD_READABLE)
class Solution {
private:
    using permutations = std::vector<std::vector<int>>;

private:
    const std::vector<int> * m_input;   /* temp pointer - to reduce call stack consuption */
    permutations * m_result;            /* temp pointer */
    std::vector<bool> m_available;

public:
    std::vector<std::vector<int>> permute(const std::vector<int>& nums) {
        permutations result;

        m_input = &nums;
        m_result = &result;
        m_available = std::vector<bool>(nums.size(), true);

        traverse({});

        return result;
    }

private:
    void traverse(const std::vector<int> & p_permutation) {
        bool is_leaf = true;

        for (std::size_t i = 0; i < m_input->size(); i++) { /* O(N) */
            if (m_available[i]) {
                m_available[i] = false;

                std::vector<int> next_state(p_permutation);
                next_state.push_back(m_input->at(i));

                traverse(next_state);   /* O(N - 1) - recursion */
                is_leaf = false;

                m_available[i] = true;
            }
        }

        if (is_leaf) {
            m_result->push_back(p_permutation);
        }
    }
};
#else
class Solution {
public:
    std::vector<std::vector<int>> permute(std::vector<int>& nums) {
        std::vector<std::vector<int>> result;
        traverse(0, nums, result);
        return result;
    }

private:
    void traverse(const std::size_t p_index, std::vector<int> & p_input, std::vector<std::vector<int>> & p_result) {
        if (p_index >= p_input.size()) {
            p_result.push_back(p_input); // leaf
            return;
        }

        for (std::size_t i = p_index; i < p_input.size(); i++) {
            std::swap(p_input[i], p_input[p_index]);
            traverse(p_index + 1, p_input, p_result);
            std::swap(p_input[i], p_input[p_index]);
        }
    }
};
#endif