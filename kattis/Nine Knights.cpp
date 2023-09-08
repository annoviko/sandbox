#include <iostream>
#include <iostream>
#include <string>
#include <vector>


const int SIDE_SIZE = 5;


bool is_knight_here(const std::vector<std::string>& p_field, int r, int c) {
    if (r >= 0 && r < SIDE_SIZE && c >= 0 && c < SIDE_SIZE) {
        return p_field[r][c] == 'k';
    }

    return false;
}


struct position {
    int row = 0;
    int col = 0;
};


std::vector<position> get_movements(int r, int c) {
    std::vector<position> result = {
        { r + 2, c + 1 },
        { r + 2, c - 1 },
        { r + 1, c + 2 },
        { r + 1, c - 2 },
        { r - 1, c + 2 },
        { r - 1, c - 2 },
        { r - 2, c + 1 },
        { r - 2, c - 1 }
    };

    return result;
}


int main() {
    std::vector<std::string> field;
    for (int i = 0; i < SIDE_SIZE; i++) {
        std::string line;
        std::getline(std::cin, line);

        field.push_back(line);
    }

    int knight_counter = 0;
    for (int i = 0; i < SIDE_SIZE; i++) {
        for (int j = 0; j < SIDE_SIZE; j++) {
            if (field[i][j] != 'k') {
                continue;
            }

            knight_counter++;

            auto movements = get_movements(i, j);
            for (auto& move : movements) {
                if (is_knight_here(field, move.row, move.col)) {
                    std::cout << "invalid" << std::endl;
                    return 0;
                }
            }
        }
    }

    std::cout << ((knight_counter == 9) ? "valid" : "invalid") << std::endl;
    return 0;
}
