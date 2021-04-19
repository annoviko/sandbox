class Solution {
public:
    void rotate(std::vector<std::vector<int>>& matrix) {
        // transpose matrix - O(n)
        for (std::size_t i = 0; i < matrix.size(); i++) {
            for (std::size_t j = i + 1; j < matrix.size(); j++) {
                std::swap(matrix[i][j], matrix[j][i]);
            }
        }

        // reverse rows - O(n)
        for (auto & row : matrix) {
            for (long long i = 0, j = matrix.size() - 1; i < j; i++, j--) {
                std::swap(row[i], row[j]);
            }
        }

        // Final 2 * O(n) -> O(n)
    }
};