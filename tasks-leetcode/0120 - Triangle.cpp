class Solution {
public:
    int minimumTotal(const std::vector<std::vector<int>> & triangle) {
        if (triangle.empty()) {
            return 0;
        }

        int minimum = triangle[0][0];
        std::vector<int> previous_costs = triangle[0];

        for (std::size_t i = 1; i < triangle.size(); i++) {     /* time: O(levels) */
            std::vector<int> current_costs(triangle[i].size(), std::numeric_limits<int>::max()); /* space: O(N) */

            minimum = std::numeric_limits<int>::max();
            const std::vector<int> & current_level = triangle[i];

            for (int j = 0; j < current_level.size(); j++) {  /* time: O(elements on levels) * O(levels) = O(N) */
                const int step_cost = current_level[j];

                if (j == 0) {   /* left node */
                    current_costs[j] = std::min(previous_costs.front() + step_cost, current_costs[j]);
                }
                else if (j == current_level.size() - 1) { /* right node */
                    current_costs[j] = std::min(previous_costs.back() + step_cost, current_costs[j]);
                }
                else
                { /* node in between */
                    current_costs[j] = std::min(previous_costs[j - 1] + step_cost, current_costs[j]);
                    current_costs[j] = std::min(previous_costs[j] + step_cost, current_costs[j]);
                }

                minimum = std::min(current_costs[j], minimum);
            }

            previous_costs = std::move(current_costs);
        }

        return minimum;
    }
};