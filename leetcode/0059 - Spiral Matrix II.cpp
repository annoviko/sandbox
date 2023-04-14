#include <vector>


enum class direction_t {
    RIGHT,
    DOWN,
    LEFT,
    UP
};


struct step {
    int row = 0;
    int col = 0;
    direction_t dir = direction_t::RIGHT;
};


using matrix = std::vector<std::vector<int>>;


class Solution {
private:
    step get_next_step(const matrix& mtrx, direction_t dir, int row, int col) {
        switch (dir) {
        case direction_t::RIGHT:
            col++;
            if ((col >= mtrx[0].size()) || (mtrx[row][col] > 0)) {
                dir = direction_t::DOWN;
                col--;
            }

            break;

        case direction_t::DOWN:
            row++;
            if ((row >= mtrx.size()) || (mtrx[row][col] > 0)) {
                dir = direction_t::LEFT;
                row--;
            }

            break;

        case direction_t::LEFT:
            col--;
            if ((col < 0) || (mtrx[row][col] > 0)) {
                dir = direction_t::UP;
                col++;
            }

            break;

        case direction_t::UP:
            row--;
            if ((row < 0) || (mtrx[row][col] > 0)) {
                dir = direction_t::RIGHT;
                row++;
            }

            break;
        }

        return { row, col, dir };
    }

public:
    std::vector<std::vector<int>> generateMatrix(int n) {
        matrix mtrx = matrix(n, std::vector<int>(n, -1));

        int row = 0;
        int col = 0;
        int value = 1;
        direction_t dir = direction_t::RIGHT;

        while (true) {
            mtrx[row][col] = value;
            value++;

            auto pos = get_next_step(mtrx, dir, row, col);
            if ((pos.row == row) && (pos.col == col)) {

                pos = get_next_step(mtrx, pos.dir, row, col);
                if ((pos.row == row) && (pos.col == col)) {
                    return mtrx;
                }
            }

            row = pos.row;
            col = pos.col;
            dir = pos.dir;
        }
    }
};