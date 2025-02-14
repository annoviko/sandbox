#include <iostream>
#include <set>
#include <vector>


int main() {
    int n, k;
    std::cin >> n >> k;

    std::vector<int> gnomes;
    std::set<int> missing;

    for (int i = 1; i <= n; i++) {
        missing.insert(i);
    }

    for (int i = 0; i < k; i++) {
        int value = 0;
        std::cin >> value;

        gnomes.push_back(value);
        missing.erase(value);
    }

    for (int cur_index = 0; cur_index < gnomes.size(); cur_index++) {
        while (!missing.empty() && (*missing.begin() < gnomes[cur_index])) {
            std::cout << *missing.begin() << std::endl;
            missing.erase(missing.begin());
        }

        std::cout << gnomes[cur_index] << std::endl;
    }

    while (!missing.empty()) {
        std::cout << *missing.begin() << std::endl;
        missing.erase(missing.begin());
    }

    return 0;
}