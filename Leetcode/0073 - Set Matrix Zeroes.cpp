#if defined(ADDITIONAL_MEMORY)
struct Position {
    int row = 0;
    int col = 0;
};

class Solution {
public:
    void setZeroes(std::vector<std::vector<int>>& matrix) {
        std::size_t rows = matrix.size();
        std::size_t cols = matrix[0].size();
        
        std::list<Position> zeros;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (matrix[i][j] == 0) {
                    zeros.push_back({i, j});
                }
            }
        }
        
        for (const auto & pos : zeros) {
            for (int i = 0; i < rows; i++) {
                matrix[i][pos.col] = 0;
            }
            
            for (int i = 0; i < cols; i++) {
                matrix[pos.row][i] = 0;
            }
        }
    }
};
#endif

class Solution {
public:
    void setZeroes(std::vector<std::vector<int>>& matrix) {
        bool is_zero_col = false;
        bool is_zero_row = false;
        
        for (int i = 0; i < matrix.size(); i++) {
            if (matrix[i][0] == 0) {
                is_zero_col = true;
            }
            
            for (int j = 1; j < matrix[0].size(); j++) {
                if (matrix[i][j] == 0) {
                    matrix[0][j] = 0;
                    matrix[i][0] = 0;
                }
            }
        }
        
        for (int i = 1; i < matrix.size(); i++) {
            for (int j = 1; j < matrix[0].size(); j++) {
                if (matrix[i][0] == 0 || matrix[0][j] == 0) {
                    matrix[i][j] = 0;
                }
            }
        }
        
        if (matrix[0][0] == 0) {          
            for (int i = 0; i < matrix[0].size(); i++) {
                matrix[0][i] = 0;
            }
        }
        
        if (is_zero_col) {
            for (int i = 0; i < matrix.size(); i++) {
                matrix[i][0] = 0;
            }
        }
    }
};