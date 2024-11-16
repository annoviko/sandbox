#include <iostream>
#include <vector>


std::vector<int> find_rational(int n) {
    std::vector<int> path;
    while (n != 1) {
        path.push_back(n);

        if (n % 2 == 0) {
            n = n / 2;
        }
        else {
            n = (n - 1) / 2;
        }
    }

    std::vector<int> r = { 1, 1 };
    for (auto iter = path.rbegin(); iter != path.rend(); iter++) {
        int index = *iter;

        if (index % 2 == 0) {
            r = { r[0], r[0] + r[1] };
        }
        else {
            r = { r[0] + r[1], r[1] };
        }
    }

    return r;
}


int main() {
    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        int id, idx;
        std::cin >> id >> idx;

        auto r = find_rational(idx);

        std::cout << id << ' ' << r[0] << '/' << r[1] << '\n';
    }

    return 0;
}
