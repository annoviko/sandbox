#include <vector>

class Solution {
public:
    std::vector<int> countBits(int n) {
        std::vector<int> result(n + 1, 0);
        for (int i = 1; i <= n; ++i) {
            if (i % 2 == 0) {
                result[i] = result[i >> 1];
            }
            else {
                result[i] = result[i - 1] + 1;
            }
        }

        return result;
    }
};