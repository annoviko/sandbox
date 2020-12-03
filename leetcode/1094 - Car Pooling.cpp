class Solution {
public:
#ifdef REGULAR_SORT_VIA_MAP
    bool carPooling(const std::vector<std::vector<int>>& trips, int capacity) {
        if (trips.empty()) { return true; }

        std::map<int, int> capacity_evolution;   /* sorted collections: { stop : change } */

        for (const auto & trip : trips) {   /* O(N * log(N)) */
            capacity_evolution[trip[1]] += trip[0];
            capacity_evolution[trip[2]] -= trip[0];
        }

        int current_capacity = 0;
        for (const auto & stop : capacity_evolution) { /* O(N) */
            current_capacity += stop.second;
            if (current_capacity > capacity) {
                return false;
            }
        }

        return true;
    }
#else
    /* there is restriction that amount of trips couldn't be bigger than 1001 */
    bool carPooling(const std::vector<std::vector<int>>& trips, int capacity) {
        if (trips.empty()) { return true; }

        std::vector<int> capacity_evolution(1001, 0);   /* sorted collections: { stop : change } */

        for (const auto & trip : trips) {   /* O(N) */
            capacity_evolution[trip[1]] += trip[0];
            capacity_evolution[trip[2]] -= trip[0];
        }

        int current_capacity = 0;
        for (const auto & change : capacity_evolution) { /* O(1) */
            current_capacity += change;
            if (current_capacity > capacity) {
                return false;
            }
        }

        return true;
    }
#endif
};