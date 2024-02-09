#include <string>
#include <vector>
#include <unordered_map>



class Solution {
public:
    int countSquares(const std::vector<std::vector<int>>& matrix) {
        std::vector<std::vector<int>> dp(matrix.size(), std::vector<int>(matrix[0].size(), 0));

        for (int row = 0; row < matrix.size(); row++) {
            dp[row][0] = matrix[row][0];
        }

        for (int col = 0; col < matrix[0].size(); col++) {
            dp[0][col] = matrix[0][col];
        }

        for (int row = 1; row < dp.size(); row++) {
            for (int col = 1; col < dp[row].size(); col++) {
                if (matrix[row][col] != 0) {
                    int min_value = std::min(dp[row - 1][col], std::min(dp[row - 1][col - 1], dp[row][col - 1]));
                    dp[row][col] = min_value + 1;
                }
            }
        }

        int result = 0;
        for (auto& row : dp) {
            for (int value : row) {
                result += value;
            }
        }

        return result;
    }
};
