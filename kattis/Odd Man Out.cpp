#include <iostream>
#include <unordered_set>


int main() {
    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::unordered_set<int> alone_guests;

        int k = 0;
        std::cin >> k;

        for (int j = 0; j < k; j++) {
            int id = 0;
            std::cin >> id;

            auto iter = alone_guests.find(id);
            if (iter == alone_guests.cend()) {
                alone_guests.insert(id);
            }
            else {
                alone_guests.erase(iter);
            }
        }

        std::cout << "Case #" << i + 1 << ": " << *(alone_guests.begin()) << std::endl;
    }

    return 0;
}