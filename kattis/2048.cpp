#include <algorithm>
#include <iostream>
#include <vector>


std::vector<int> merge(const std::vector<int>& ar) {
    std::vector<int> result(4, 0);

    int index = -1;
    for (int i = 0; i < 4; i++) {
        if (ar[i] == 0) {
            continue;
        }

        if (index < 0) {
            result[++index] = ar[i];
        }
        else if (result[index] == ar[i]) {
            result[index++] <<= 1;
        }
        else {
            if (result[index] == 0) {
                result[index] = ar[i];
            }
            else {
                result[++index] = ar[i];
            }
        }
    }

    return result;
}


const int LEFT = 0;
const int UP = 1;
const int RIGHT = 2;
const int DOWN = 3;


int main() {
    merge({ 2, 4, 2, 2 });

    std::vector<std::vector<int>> field;

    for (int i = 0; i < 4; i++) {
        field.push_back(std::vector<int>(4, 0));
        for (int j = 0; j < 4; j++) {
            std::cin >> field[i][j];
        }
    }

    int direction = 0;
    std::cin >> direction;

    std::vector<std::vector<int>> result(4, std::vector<int>(4, 0));
    switch (direction) {
        case LEFT: {
            for (int i = 0; i < 4; i++) {
                result[i] = merge(field[i]);
            }
            break;
        }

        case RIGHT: {
            for (int i = 0; i < 4; i++) {
                std::vector<int> row_in;
                for (int j = 0; j < 4; j++) {
                    row_in.push_back(field[i][field.size() - 1 - j]);
                }

                auto row_out = merge(row_in);
                std::reverse(row_out.begin(), row_out.end());
                result[i] = row_out;
            }
            break;
        }

        case UP: {
            for (int c = 0; c < 4; c++) {
                std::vector<int> col_in;
                for (int r = 0; r < 4; r++) {
                    col_in.push_back(field[r][c]);
                }

                auto col_out = merge(col_in);
                for (int r = 0; r < 4; r++) {
                    result[r][c] = col_out[r];
                }
            }
            break;
        }

        case DOWN: {
            for (int c = 0; c < 4; c++) {
                std::vector<int> col_in;
                for (int r = 0; r < 4; r++) {
                    col_in.push_back(field[field.size() - 1 - r][c]);
                }

                auto col_out = merge(col_in);
                for (int r = 0; r < 4; r++) {
                    result[field.size() - 1 - r][c] = col_out[r];
                }
            }
            break;
        }
    }

    for (const auto & r : result) {
        bool is_first = true;
        for (auto v : r) {
            if (!is_first) {
                std::cout << ' ';
            }
            std::cout << v;
            is_first = false;
        }
        std::cout << std::endl;
    }

    return 0;
}