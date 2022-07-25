class Solution {
public:
#if defined (BINARY_SEARCH) // O(M * log(N))
    bool searchMatrix(const std::vector<std::vector<int>>& matrix, int target) {
        std::size_t limit = matrix.size() - 1;
        for (std::size_t i = 0; (i < matrix[0].size()) && (matrix[0][i] <= target); i++) {
            if (findTargetInColumn(matrix, i, target, limit)) {
                return true;
            }
        }

        return false;
    }

    bool findTargetInColumn(const std::vector<std::vector<int>>& matrix, std::size_t col_index, int target, std::size_t & limit) {
        std::size_t left = 0;
        std::size_t right = limit;

        while (left < right) {
            std::size_t middle = (right + left) / 2;

            if (matrix[middle][col_index] < target) {
                left = middle + 1;
            }
            else if (matrix[middle][col_index] > target) {
                right = middle - 1;
            }
            else {
                return true;
            }
        }

        limit = right;
        return matrix[left][col_index] == target;
    }
#endif
    // O(N + M)
    bool searchMatrix(const std::vector<std::vector<int>>& matrix, int target) {
        int col = (int)matrix[0].size() - 1;
        int row = 0;

        while ((col >= 0) && (row < matrix.size())) {
            if (matrix[row][col] > target) {
                col--;
            }
            else if (matrix[row][col] < target) {
                row++;
            }
            else {
                return true;
            }
        }

        return false;
    }
};