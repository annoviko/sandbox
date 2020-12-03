class Solution {
public:
    int twoCitySchedCost(std::vector<std::vector<int>>& costs) {
        int total_distance = std::accumulate(costs.begin(), costs.end(), 0, [](int sum, std::vector<int> & p_way) {
            return sum + p_way[0];
        });

        /*
        
        distance to city B - distance to city A

        - if distance is negative than it is much better to go to B
        - if distance is positive than it is much better to go to A
        
        */

        std::vector<int> difference(costs.size(), 0);
        for (std::size_t i = 0; i < costs.size(); i++) {
            difference[i] = costs[i][1] - costs[i][0];
        }

        std::sort(difference.begin(), difference.end());

        /*

        We should substruct the most optimal ways to B

        */

        for (std::size_t i = 0; i < costs.size() / 2; i++) {
            total_distance += difference[i];
        }

        return total_distance;
    }
};