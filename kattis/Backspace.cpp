#include <iostream>
#include <list>
#include <string>


int main() {
    std::string content;
    std::cin >> content;

    std::list<char> result;

    for (char c : content) {
        if (c == '<') {
            if (!result.empty()) {
                result.pop_back();
            }
        }
        else {
            result.push_back(c);
        }
    }

    for (char c : result) {
        std::cout << c;
    }

    std::cout << std::endl;

    return 0;
}