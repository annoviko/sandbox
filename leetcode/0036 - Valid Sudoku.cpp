#include <vector>

class Solution {
public:
    bool isValidSudoku(std::vector<std::vector<char>>& board) {
        std::vector<std::vector<bool>> row_values(9, std::vector<bool>(9, false));
        std::vector<std::vector<bool>> col_values(9, std::vector<bool>(9, false));

        for (int row_block = 0; row_block < 3; row_block++) {
            for (int col_block = 0; col_block < 3; col_block++) {
                std::vector<bool> block_values(9, false);

                for (int _r = 0; _r < 3; _r++) {
                    const int r = row_block * 3 + _r;

                    for (int _c = 0; _c < 3; _c++) {
                        const int c = col_block * 3 + _c;

                        if (board[r][c] == '.') {
                            continue;
                        }

                        int value = (board[r][c] - '0') - 1;
                        if (row_values[r][value]) {
                            return false;
                        }
                        if (col_values[c][value]) {
                            return false;
                        }
                        if (block_values[value]) {
                            return false;
                        }

                        row_values[r][value] = true;
                        col_values[c][value] = true;
                        block_values[value] = true;
                    }
                }
            }
        }

        return true;
    }
};