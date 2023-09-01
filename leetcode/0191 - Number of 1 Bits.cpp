#include <cmath>
#include <vector>


class Solution {
public:
    std::vector<int> countBits(int n) {
        std::vector<int> result(n + 1, 0);

        for (int i = 1; i < n + 1; i++) {
            int degree = std::log2(i);
            int base = std::pow(2, degree);

            result[i] = 1 + result[i - base];
        }

        return result;
    }
};
