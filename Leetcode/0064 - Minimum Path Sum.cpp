class Solution {
public:
    int minPathSum(std::vector<std::vector<int>>& grid) {
        if (grid.empty()) { return 0; }
        
        std::vector<int> row_score(grid[0].size());
        for (std::size_t i = 0; i < grid.size(); i++) {
            row_score[0] = grid[i][0] + (i > 0 ? row_score[0] : 0);
            for (std::size_t j = 1; j < row_score.size(); j++) {
                int upper_score = i > 0 ? row_score[j] : INT_MAX;
                
                row_score[j] = grid[i][j] + std::min(row_score[j - 1], upper_score);
            }
        }
        
        return row_score.back();
    }
};