#include <algorithm>
#include <iostream>
#include <vector>


int main() {
    int n = 0;
    std::cin >> n;

    std::vector<int> original(n, 0);
    for (int i = 0; i < n; i++) {
        std::cin >> original[i];
    }

    auto sorted = original;
    std::sort(sorted.begin(), sorted.end());

    int l = -1;
    for (int i = 0; i < original.size(); i++) {
        if (original[i] != sorted[i]) {
            l = i;
            break;
        }
    }

    int r = -1;
    for (int i = original.size() - 1; i > l; i--) {
        if (original[i] != sorted[i]) {
            r = i;
            break;
        }
    }

    if (l == -1 || r == -1) {
        /* cards in right place, no differences */
        int middle = n / 2 + 1; /* count from 1 */
        std::cout << middle << " " << middle << std::endl;
        return 0;
    }

    /* check if it is the same (l -> r) == (r -> l) */
    for (int i = 0; i <= (r - l); i++) {
        if (original[l + i] != sorted[r - i]) {
            std::cout << "impossible" << std::endl;
            return 0;
        }
    }

    std::cout << l + 1 << " " << r + 1 << std::endl;
    return 0;
}
