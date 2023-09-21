#include <algorithm>
#include <iostream>
#include <vector>


int main() {
    while (true) {
        std::vector<int> s(3);
        std::cin >> s[0] >> s[1] >> s[2];

        if (s[0] == 0 && s[1] == 0 && s[2] == 0) {
            break;
        }

        std::sort(s.begin(), s.end());

        if (s[2] * s[2] == s[0] * s[0] + s[1] * s[1]) {
            std::cout << "right" << std::endl;
        }
        else {
            std::cout << "wrong" << std::endl;
        }
    }

    return 0;
}