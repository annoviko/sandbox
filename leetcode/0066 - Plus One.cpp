class Solution {
public:
    std::vector<int> plusOne(const std::vector<int>& digits) {
        std::vector<int> result = digits;

        bool carry_bit = true;
        for (auto iter = result.rbegin(); (iter != result.rend()) && carry_bit; iter++) {
            if (*iter != 9) {
                carry_bit = false;
                (*iter)++;
            }
            else {
                *iter = 0;
            }
        }

        if (carry_bit) {
            std::vector<int> extended_result(result.size() + 1, 1);
            for (std::size_t i = 0; i < result.size(); i++) {
                extended_result[i + 1] = result[i];
            }

            result = std::move(extended_result);
        }

        return result;
    }
};