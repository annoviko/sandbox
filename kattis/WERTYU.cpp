#include <iostream>
#include <string>
#include <vector>


int main() {
    int dict_row[255] = { -1 };
    int dict_pos[255] = { -1 };

    std::vector<std::string> keyboard = {
        "`1234567890-=",
        "QWERTYUIOP[]\\",
        "ASDFGHJKL;'",
        "ZXCVBNM,./"
    };

    for (int i = 0; i < keyboard.size(); i++) {
        for (int j = 0; j < keyboard[i].size(); j++) {
            const char c = keyboard[i][j];
            dict_row[c] = i;
            dict_pos[c] = j;
        }
    }

    for (std::string line; std::getline(std::cin, line) && !line.empty(); ) {
        std::string output;

        for (char c : line) {
            if (c == ' ') {
                output.push_back(' ');
            }
            else {
                const int index_row = dict_row[c];
                if (index_row == -1) {
                    continue;
                }

                const int index_pos = dict_pos[c];
                if (index_pos < 1) {
                    continue;
                }

                output.push_back(keyboard[index_row][index_pos - 1]);
            }
        }

        std::cout << output << std::endl;
    }

    return 0;
}