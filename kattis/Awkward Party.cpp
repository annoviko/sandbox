#include <vector>
#include <iostream>
#include <unordered_map>


int main() {
    int n = 0;
    std::cin >> n;

    std::unordered_map<int, int> awkwardness;

    int level = n;

    for (int i = 0; i < n; i++) {
        int lang = 0;
        std::cin >> lang;

        auto iter = awkwardness.find(lang);
        if (iter != awkwardness.cend()) {
            int lang_level = i - iter->second;
            level = std::min(lang_level, level);
        }

        awkwardness[lang] = i;
    }

    std::cout << level << std::endl;

    return 0;
}