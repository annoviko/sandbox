#include <iostream>
#include <vector>


class Solution {
public:
    int numSpecial(const std::vector<std::vector<int>>& mat) {
        std::vector<int> count_row(mat.size(), 0);
        std::vector<int> count_col(mat[0].size(), 0);

        for (int r = 0; r < mat.size(); r++) {
            for (int c = 0; c < mat[r].size(); c++) {
                if (mat[r][c] == 1) {
                    count_row[r]++;
                    count_col[c]++;
                }
            }
        }

        int counter = 0;
        for (int r = 0; r < mat.size(); r++) {
            for (int c = 0; c < mat[r].size(); c++) {
                if ((mat[r][c] == 1) && (count_row[r] == 1) && (count_col[c] == 1)) {
                    counter++;
                }
            }
        }

        return counter;
    }
};