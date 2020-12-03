class Solution {
public:
    int uniquePathsWithObstacles(std::vector<std::vector<int>>& obstacleGrid) {
        std::vector<long long> row(obstacleGrid[0].size(), 0);
        for (std::size_t i = 0; i < row.size(); i++) {
            if (obstacleGrid[0][i] != 1) {
                row[i] = 1;
            }
            else {
                break;
            }
        }
        
        for (std::size_t i = 1; i < obstacleGrid.size(); i++) {
            for (std::size_t j = 0; j < row.size(); j++) {
                if (obstacleGrid[i][j] == 1) {
                    row[j] = 0;
                }
                else {
                    row[j] = (j > 0) ? row[j - 1] + row[j] : row[j];
                }
            }
        }
        
        return row.back();
    }
};