#include <iostream>
#include <string>
#include <unordered_set>


int main() {
    int p, n;
    std::cin >> p >> n;

    std::unordered_set<std::string> replaced_parts;

    int i = 0;
    for (; i < n; i++) {
        std::string part_name;
        std::cin >> part_name;

        replaced_parts.insert(part_name);
        if (replaced_parts.size() == p) {
            break;
        }
    }

    if (replaced_parts.size() == p) {
        std::cout << i + 1 << std::endl;
    }
    else {
        std::cout << "paradox avoided" << std::endl;
    }

    return 0;
}