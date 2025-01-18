#include <iostream>
#include <vector>
#include <algorithm>


int main() {
    std::int64_t s, c, k;
    std::cin >> s >> c >> k;

    std::vector<std::int64_t> colors(s);
    for (int i = 0; i < s; i++) {
        std::cin >> colors[i];
    }

    std::sort(colors.begin(), colors.end());

    std::int64_t counter = 1, machines = 1;
    for (int i = 1; i < colors.size(); i++) {
        if (colors[i] - colors[i - 1] <= k) {
            counter++;

            if (counter > c) {
                /* the machine is full, we cannot use it anymore, we need the next one */
                machines++;
                counter = 1;
            }
        }
        else {
            /* the closest color is not suitable, we cannot wash it in the same machine, we need the next one */
            machines++;
            counter = 1;
        }
    }

    std::cout << machines << std::endl;
    return 0;
}