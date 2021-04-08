#include <vector>


class Solution {
public:
    std::vector<std::vector<int>> generate(int numRows) {
        std::vector<std::vector<int>> result = { };

        std::vector<int> prev = { };
        std::vector<int> cur = { 1 };

        result.push_back(cur);

        for (int i = 1; i < numRows; i++)
        {
            prev = std::move(cur);

            cur = std::vector<int>(prev.size() + 1);
            for (int j = 0; j < cur.size(); j++) {
                int left = (j - 1 >= 0) ? prev[j - 1] : 0;
                int right = (j < prev.size()) ? prev[j] : 0;

                cur[j] = left + right;
            }

            result.push_back(cur);
        }

        return result;
    }
};