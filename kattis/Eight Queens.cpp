#include <iostream>
#include <string>
#include <vector>


bool check_row(const std::vector<std::string>& field, int row, int col) {
    for (int i = 0; i < 8; i++) {
        if (i != col && field[row][i] == '*') {
            return false;
        }
    }

    return true;
}


bool check_col(const std::vector<std::string>& field, int row, int col) {
    for (int i = 0; i < 8; i++) {
        if (i != row && field[i][col] == '*') {
            return false;
        }
    }

    return true;
}


bool check_diag1(const std::vector<std::string>& field, int row, int col) {
    int r = row - 1, c = col - 1;

    for (; r >= 0 && c >= 0; r--, c--) {
        if (field[r][c] == '*') {
            return false;
        }
    }

    r = row + 1, c = col + 1;
    for (; r < 8 && c < 8; r++, c++) {
        if (field[r][c] == '*') {
            return false;
        }
    }

    return true;
}


bool check_diag2(const std::vector<std::string>& field, int row, int col) {
    int r = row - 1, c = col + 1;

    for (; r >= 0 && c < 8; r--, c++) {
        if (field[r][c] == '*') {
            return false;
        }
    }

    r = row + 1, c = col - 1;
    for (; r < 8 && c >= 0; r++, c--) {
        if (field[r][c] == '*') {
            return false;
        }
    }

    return true;
}


bool check_queen(const std::vector<std::string>& field, int row, int col) {
    return check_row(field, row, col) && check_col(field, row, col) && check_diag1(field, row, col) && check_diag2(field, row, col);
}


int main() {
    std::vector<std::string> field(8, std::string());

    for (int i = 0; i < 8; i++) {
        std::getline(std::cin, field[i]);
    }

    int queen_counter = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (field[i][j] == '*') {
                queen_counter++;
                if (!check_queen(field, i, j)) {
                    std::cout << "invalid" << std::endl;
                    return 0;
                }
            }
        }
    }

    if (queen_counter != 8) {
        std::cout << "invalid" << std::endl;
    }
    else {
        std::cout << "valid" << std::endl;
    }

    return 0;
}