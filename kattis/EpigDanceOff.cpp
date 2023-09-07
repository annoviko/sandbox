#include <iostream>
#include <string>
#include <vector>


int main() {
    int n, m;
    std::cin >> n >> m;

    std::vector<std::string> field(n);

    for (int i = 0; i < n; i++) {
        std::cin >> field[i];
    }

    int moves = 0;
    bool is_dancing = false;

    for (int col = 0; col < m; col++) {
        is_dancing = false;
        for (int row = 0; (row < n) && (!is_dancing); row++) {
            if (field[row][col] == '$') {
                is_dancing = true;
                continue; /* still dancing */
            }
        }

        if (!is_dancing) {
            moves++;
        }
    }

    if (is_dancing) {
        /* if dancing in the last frame - then we count it */
        moves++;
    }

    std::cout << moves << std::endl;

    return 0;
}