/*

{ 1, 2, 3 }

max = 2^len

i
000 - null
001 - 3
010 - 2
011 - 2 3
100 - 1
101 - 1 3
110 - 1 2
111 - 1 2 3

*/


class Solution {
public:
    std::vector<std::vector<int>> subsets(std::vector<int>& nums) {
        std::vector<std::vector<int>> result;
        const int limit = std::pow(2, nums.size());

        for (std::size_t i = 0; i < limit; i++) {
            std::vector<int> subset = { };

            for (std::size_t j = 0; j < nums.size(); j++) {
                const std::size_t mask = 1 << j;
                if ((mask & i) != 0) {
                   subset.push_back(nums[j]);
                }
            }

            result.push_back(std::move(subset));
        }

        return result;
    }
};