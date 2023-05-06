#include <iostream>
#include <unordered_set>



int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

    while (true) {
        int n, m;
        std::cin >> n >> m;

        if (n == 0 && m == 0) {
            break;
        }

        std::unordered_set<int> one_collection;
        for (int i = 0; i < n; i++) {
            int title;
            std::cin >> title;
            one_collection.insert(title);
        }

        int sold_cds = 0;
        for (int i = 0; i < m; i++) {
            int title;
            std::cin >> title;

            auto iter = one_collection.find(title);
            if (iter != one_collection.cend()) {
                sold_cds++;
                one_collection.erase(title);
            }
        }

        std::cout << sold_cds << std::endl;
    }

    return 0;
}