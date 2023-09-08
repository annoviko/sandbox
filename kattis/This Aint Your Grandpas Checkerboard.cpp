#include <iostream>
#include <string>
#include <vector>


int main() {
    int n = 0;
    std::cin >> n;

    std::vector<std::string> field(n);
    for (int i = 0; i < n; i++) {
        std::cin >> field[i];
    }

    for (int i = 0; i < n; i++) {
        int seq = 1;
        int black = 0;
        int white = 0;

        char last = 0;

        for (int j = 0; j < n; j++) {
            if (field[i][j] == 'W') {
                white++;
            }
            else {
                black++;
            }

            if (field[i][j] == last) {
                seq++;
                if (seq >= 3) {
                    std::cout << 0 << std::endl;
                    return 0;
                }
            }
            else {
                seq = 1;
            }

            last = field[i][j];
        }

        if (black != white || seq >= 3) {
            std::cout << 0 << std::endl;
            return 0;
        }
    }

    for (int i = 0; i < n; i++) {
        int seq = 1;
        int black = 0;
        int white = 0;

        char last = 0;

        for (int j = 0; j < n; j++) {
            if (field[j][i] == 'W') {
                white++;
            }
            else {
                black++;
            }

            if (field[j][i] == last) {
                seq++;
                if (seq >= 3) {
                    std::cout << 0 << std::endl;
                    return 0;
                }
            }
            else {
                seq = 1;
            }

            last = field[j][i];
        }

        if (black != white || seq >= 3) {
            std::cout << 0 << std::endl;
            return 0;
        }
    }

    std::cout << 1 << std::endl;
    return 0;
}