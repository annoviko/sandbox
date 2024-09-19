#include <list>
#include <iostream>


int main() {
    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::list<int> stack;
        int counter = 0;

        int id = 0;
        std::cin >> id;

        for (int j = 0; j < 12; j++) {
            int value = 0;
            std::cin >> value;

            if (stack.empty()) {
                if (value > 0) {
                    stack.push_back(value);
                    counter++;
                }

                continue;
            }


            if (value == 0) {
                stack.clear();
            }
            else if (value > stack.back()) {
                stack.push_back(value);
                counter++;
            }
            else if (value < stack.back()) {
                while (!stack.empty() && stack.back() > value) {
                    stack.pop_back();
                }

                if (stack.empty() || stack.back() < value) {
                    stack.push_back(value);
                    counter++;
                }
            }
        }

        std::cout << id << ' ' << counter << std::endl;
    }

    return 0;
}