#include <algorithm>
#include <vector>
#include <iostream>


int main() {
    while (true) {
        int n, m;
        std::cin >> n >> m;

        if (n == 0 && m == 0) {
            break;
        }

        std::vector<int> heads(n);
        std::vector<int> knights(m);

        for (int i = 0; i < n; i++) {
            std::cin >> heads[i];
        }

        for (int i = 0; i < m; i++) {
            std::cin >> knights[i];
        }

        std::sort(heads.begin(), heads.end());
        std::sort(knights.begin(), knights.end());

        int price = 0;
        int i = 0, j = 0;
        while (i < heads.size() && j < knights.size()) {
            if (heads[i] <= knights[j]) {
                price += knights[j];
                i++;
            }

            j++;
        }

        if (i == heads.size()) {
            std::cout << price << std::endl;
        }
        else {
            std::cout << "Loowater is doomed!" << std::endl;
        }
    }

    return 0;
}
