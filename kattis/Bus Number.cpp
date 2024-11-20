#include <iostream>
#include <vector>


int main() {
    int n;
    std::cin >> n;

    std::vector<bool> buses(1000 + 1, false);

    for (int i = 0; i < n; i++) {
        int bus;
        std::cin >> bus;

        buses[bus] = true;
    }

    bool is_first = true;
    int begin = -1, end = -1;
    for (int i = 0; i < buses.size(); i++) {
        if (buses[i]) {
            if (begin == -1) {
                begin = i;
                
                if (!is_first) {
                    std::cout << ' ';
                }
                std::cout << i;
                is_first = false;
            }
            else {
                end = i;
            }
        }
        else {
            if (end != -1) {
                std::cout << ((end - begin > 1) ? '-' : ' ') << end;
            }

            begin = -1;
            end = -1;
        }
    }

    if (end != -1) {
        std::cout << ((end - begin > 1) ? '-' : ' ') << end;
    }

    return 0;
}