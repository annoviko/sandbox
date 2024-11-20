#include <iostream>
#include <unordered_set>


int main() {
    int r, n;
    std::cin >> r >> n;

    std::unordered_set<int> reserved;
    for (int i = 0; i < r; i++) {
        reserved.insert(i + 1);
    }

    for (int i = 0; i < n; i++) {
        int room;
        std::cin >> room;

        reserved.erase(room);
    }

    if (!reserved.empty()) {
        std::cout << *(reserved.begin()) << std::endl;
    }
    else {
        std::cout << "too late" << std::endl;
    }

    return 0;
}