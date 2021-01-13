#include <algorithm>
#include <vector>


class Solution {
public:
    int numRescueBoats(std::vector<int>& people, int limit) {
        int result = 0;

        std::sort(people.begin(), people.end(), std::greater<>());

        int lborder = 0;
        int rborder = people.size() - 1;

        while (lborder <= rborder) {
            if (people[lborder] + people[rborder] <= limit) {
                rborder--;
            }

            result++;
            lborder++;
        }

        return result;
    }
};