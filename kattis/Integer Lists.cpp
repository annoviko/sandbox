#include <iostream>
#include <string>
#include <list>


int main() {
    int t = 0;
    std::cin >> t;

    for (int i = 0; i < t; i++) {
        std::string command;
        std::cin >> command;

        int n = 0;
        std::cin >> n;

        std::string content;
        std::cin >> content;

        std::list<int> list;

        std::string value;
        for (int j = 0; j < content.size(); j++) {
            if (content[j] == '[') {
                continue;
            }
            else if (content[j] >= '0' && content[j] <= '9') {
                value.push_back(content[j]);
            }
            else if ((content[j] == ',' || content[j] == ']') && !value.empty()) {
                list.push_back(std::stoi(value));
                value.clear();
            }
        }

        auto iter = list.begin();
        bool error_detected = false;
        for (char c : command) {
            if (c == 'D') {
                if (list.empty()) {
                    std::cout << "error" << std::endl;
                    error_detected = true;
                    break;
                }

                if (iter == list.end()) {
                    iter--;
                    iter = list.erase(iter);
                }
                else {
                    iter = list.erase(iter);
                }
            }
            else if (c == 'R') {
                iter = (iter == list.begin()) ? list.end() : list.begin();
            }
        }

        if (error_detected) {
            continue;
        }

        std::cout << '[';
        bool is_first = true;

        if (iter == list.begin()) {
            for (int val : list) {
                if (!is_first) { std::cout << ','; }

                std::cout << val;
                is_first = false;
            }
        }
        else {
            for (auto cur = list.rbegin(); cur != list.rend(); cur++) {
                if (!is_first) { std::cout << ','; }

                std::cout << *cur;
                is_first = false;
            }
        }

        std::cout << ']' << std::endl;

    }

    return 0;
}