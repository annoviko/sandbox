#include <algorithm>
#include <vector>


class Solution {
public:
    int getMaximumGenerated(int n) {
        if (n < 1) { return 0; }

        std::vector<int> container(n + 1);
        container[0] = 0;
        container[1] = 1;

        int result = 1;

        for (int i = 1, basic_index = 2 * i; basic_index < container.size(); i++, basic_index = 2 * i) {
            container[basic_index] = container[i];
            result = std::max(result, container[basic_index]);

            if (basic_index + 1 < container.size()) {
                container[basic_index + 1] = container[i] + container[i + 1];
                result = std::max(result, container[basic_index + 1]);
            }
        }

        return result;
    }
};