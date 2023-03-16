#include <iostream>
#include <vector>


int main() {
    int n = 0;
    std::cin >> n;

    while (n != 0) {
        int counter = 0;
        std::vector<std::vector<bool>> map(501, std::vector<bool>(501, false));

        for (int i = 0; i < n; i++) {
            int x1, y1, x2, y2;
            std::cin >> x1 >> y1 >> x2 >> y2;

            for (int y = y1; y < y2; y++) {
                for (int x = x1; x < x2; x++) {
                    if (!map[y][x]) {
                        counter++;
                        map[y][x] = true;
                    }
                }
            }
        }

        std::cout << counter << std::endl;
        std::cin >> n;
    }
}