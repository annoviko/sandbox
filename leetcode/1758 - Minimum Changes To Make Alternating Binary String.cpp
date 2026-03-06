#include <algorithm>
#include <string>


class Solution {
public:
    int minOperations(const std::string &s) {
        auto count = [&s](bool is_one) -> int {
            int counter = 0;
            for (int i = 0; i < s.size(); i++) {
                if (is_one && s[i] != '1') {
                    counter++;
                }
                else if (!is_one && s[i] != '0') {
                    counter++;
                }

                is_one = !is_one;
            }

            return counter;
        };

        return std::min(count(true), count(false));
    }
};