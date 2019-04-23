#include <string>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>


class Solution {
public:
    int maximalSquare(std::vector<std::vector<char>>& matrix) {
        if (matrix.empty()) {
            return 0;
        }
        
        const std::size_t rows = matrix.size();
        const std::size_t cols = matrix[0].size();

        std::size_t max_edge = 0;

        std::vector<std::size_t> prev;
        std::vector<std::size_t> cur(cols);

        for (std::size_t i = 0; i < rows; i++) {
            for (std::size_t j = 0; j < cols; j++) {
                cur[j] = matrix[i][j] - '0';
                
                if (cur[j] > 0 && j > 0 && i > 0) {
                    std::vector<std::size_t> corner = {cur[j - 1], prev[j], prev[j - 1]};
                    cur[j] = *std::min_element(corner.cbegin(), corner.cend()) + 1;
                }
                
                max_edge = std::max(max_edge, cur[j]);
            }

            prev = cur;
        }

        return max_edge * max_edge;
    }
};