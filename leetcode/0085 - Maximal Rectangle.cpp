#include <vector>


class Solution {
public:
    int maximalRectangle(const std::vector<std::vector<char>>& matrix) {
        std::vector<std::vector<int>> width(matrix.size(), std::vector<int>(matrix[0].size(), 0));
        for (int r = 0; r < matrix.size(); r++) {
            int cur_width = 0;
            for (int c = 0; c < matrix[r].size(); c++) {
                if (matrix[r][c] != '1') {
                    cur_width = 0;
                }
                else {
                    cur_width++;
                }

                width[r][c] = cur_width;
            }
        }

        int max_area = 0;
        for (int c = 0; c < matrix[0].size(); c++) {    /* O(M) */
            for (int r = 0; r < matrix.size(); r++) {   /* O(N) */
                if (width[r][c] == 0) {
                    continue;
                }

                int cur_width = width[r][c];
                int cur_height = 0;

                for (int ri = r; (ri < matrix.size()) && (width[ri][c] > 0); ri++) {    /* O(N) */
                    cur_width = std::min(cur_width, width[ri][c]);
                    cur_height++;
                    max_area = std::max(cur_width * cur_height, max_area);
                }
            }
        }

        return max_area;
    }
};