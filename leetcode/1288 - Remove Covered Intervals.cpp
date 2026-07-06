#include <algorithm>
#include <vector>


class Solution {
public:
    int removeCoveredIntervals(std::vector<std::vector<int>>& intervals) {
        /* O(N*log(N)) */
        std::sort(intervals.begin(), intervals.end(), [](const std::vector<int>& l, const std::vector<int>& r) {
            if (l[0] < r[0]) {
                return true;
            }
            else if (l[0] == r[0]) {
                return l[1] > r[1];
            }
            return false;
        });

        int cur_idx = 0;
        int counter = 0;
        for (int i = 1; i < intervals.size(); i++) {    /* O(N) */
            if (intervals[cur_idx][1] >= intervals[i][1]) {
                counter++;  /* interval consumed */
            }
            else {
                cur_idx = i;    /* this one will remain - start consider it */
            }
        }

        return static_cast<int>(intervals.size()) - counter;
    }
};