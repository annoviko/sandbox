#include <vector>
#include <iostream>
#include <unordered_map>


int main() {
    int n = 0;
    std::cin >> n;

    std::unordered_map<int, int> arrows;
    int arrow_counter = 0;

    for (int i = 0; i < n; i++) {
        int height;
        std::cin >> height;

        if (arrows[height] == 0) {
            arrows[height - 1]++;  /* new arrow is needed - consider that we hit the baloni */
            arrow_counter++;
        }
        else {
            arrows[height]--;

            if (height - 1 > 0) {
                arrows[height - 1]++;
            }
        }
    }

    std::cout << arrow_counter << std::endl;

    return 0;
}