#include <vector>
#include <bit>
#include <algorithm>


class Solution {
public:
    std::vector<int> sortByBits(std::vector<int>& arr) {
        std::sort(arr.begin(), arr.end(), [](int l, int r) {
            const int lbits = std::popcount((unsigned int)l);
            const int rbits = std::popcount((unsigned int)r);

            if (lbits < rbits) {
                return true;
            }
            else if (lbits > rbits) {
                return false;
            }
            else {
                return l < r;
            }
        });

        return arr;
    }
};
