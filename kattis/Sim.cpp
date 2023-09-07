#include <iostream>
#include <sstream>
#include <string>
#include <list>


int main() {
    std::string line;
    std::getline(std::cin, line);

    int t = std::stoi(line);

    for (int i = 0; i < t; i++) {
        std::list<char> content;
        auto iter = content.begin();

        if (!std::getline(std::cin, line) || line.empty()) { 
            break;
        }

        for (char c : line) {
            if (c == '[') {
                iter = content.begin();
            }
            else if (c == ']') {
                iter = content.end();
            }
            else if (c == '<') {
                if (iter != content.begin()) {
                    iter--; /* point to the last element that should be deleted */
                    iter = content.erase(iter);  /* removes current, points to the next element */
                }
            }
            else {
                iter = content.insert(iter, c);  /* inserts value before pos and returns iterator to inserted element */
                iter++;
            }
        }

        for (char letter : content) {
            std::cout << letter;
        }

        std::cout << std::endl;
    }

    return 0;
}