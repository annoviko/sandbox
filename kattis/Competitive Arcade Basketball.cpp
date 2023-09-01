#include <iostream>
#include <string>
#include <unordered_map>


int main() {
    int n = 0, p = 0, m = 0;
    std::cin >> n >> p >> m;

    std::unordered_map<std::string, std::uint64_t> scores;

    for (int i = 0; i < n; i++) {
        std::string name;
        std::cin >> name;

        scores[name] = 0;
    }

    for (int i = 0; i < m; i++) {
        std::string name;
        std::uint64_t score;

        std::cin >> name >> score;

        auto iter = scores.find(name);
        if (iter == scores.cend()) {
            continue;   /* winner */
        }

        iter->second += score;
        if (iter->second >= p) {
            std::cout << iter->first << " wins!" << std::endl;
            scores.erase(iter);
        }
    }

    if (scores.size() == n) {
        std::cout << "No winner!" << std::endl;
    }

    return 0;
}
