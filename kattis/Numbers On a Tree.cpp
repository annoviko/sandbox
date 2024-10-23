#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>


int main() {
    std::string input;
    std::getline(std::cin, input);

    std::stringstream stream(input);

    int height = 0;
    std::string path;
    stream >> height >> path;

    int max_element = std::pow(2, height + 1) - 1;

    int cur_pos = 0;
    for (char dir : path) {
        if (dir == 'R') {
            cur_pos = 2 * cur_pos + 2;
        }
        else {
            cur_pos = 2 * cur_pos + 1;
        }
    }

    int answer = max_element - cur_pos;
    std::cout << answer << std::endl;

    return 0;
}