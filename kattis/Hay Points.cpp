#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>


int main() {
    int n, m;
    std::cin >> n >> m;

    std::unordered_map<std::string, std::uint64_t> dict;
    for (int i = 0; i < n; i++) {
        std::string name;
        std::uint64_t value = 0;

        std::cin >> name >> value;

        dict[name] = value;
    }

    for (int i = 0; i < m; i++) {
        std::uint64_t value = 0;
        for (std::string word; (std::cin >> word) && (word != ".");) {
            auto iter = dict.find(word);
            if (iter != dict.cend()) {
                value += iter->second;
            }
        }

        std::cout << value << std::endl;
    }

    return 0;
}
