#include <iostream>


int count_n(int p, int q) {
    if (p == q) {
        return 1;
    }

    if (p > q) {
        return 2 * count_n(p - q, q) + 1;
    }

    if (q > p) {
        return 2 * count_n(p, q - p);
    }
}


int main() {
    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        int id, p, q;
        std::cin >> id >> p;
        std::cin.ignore(1);
        std::cin >> q;

        std::cout << id << ' ' << count_n(p, q) << std::endl;
    }

    return 0;
}