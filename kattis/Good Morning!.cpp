#include <iostream>
#include <string>
#include <vector>


/* 

1 2 3
4 5 6
7 8 9
  0

*/


const std::vector<int> DIGIT_TO_ROW = {
    3, 0, 0, 0, 1, 1, 1, 2, 2, 2
};


const std::vector<int> DIGIT_TO_COL = {
    1, 0, 1, 2, 0, 1, 2, 0, 1, 2
};


bool is_possible(const std::string& value) {
    for (int i = 1; i < value.size(); i++) {
        const int idx1 = value[i - 1] - '0';
        const int idx2 = value[i] - '0';

        if (DIGIT_TO_ROW[idx1] > DIGIT_TO_ROW[idx2]) {
            return false;
        }

        if (DIGIT_TO_COL[idx1] > DIGIT_TO_COL[idx2]) {
            return false;
        }
    }

    return true;
}


int main() {
    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        int value;
        std::cin >> value;

        int lo = value;
        int hi = value;

        while (true) {
            if ((lo >= 0) && is_possible(std::to_string(lo))) {
                std::cout << lo << std::endl;
                break;
            }

            lo--;

            if ((hi >= 0) && is_possible(std::to_string(hi))) {
                std::cout << hi << std::endl;
                break;
            }

            hi++;
        }
    }

    return 0;
}