#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <cstdio>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n = 0;
    std::cin >> n;

    std::vector<std::list<std::string>> holder(n);

    for (int i = 0; i < n; i++) {
        std::string word;
        std::cin >> word;
        holder[i].push_back(std::move(word));
    }

    int last_non_empty = 0;
    for (int i = 0; i < n - 1; i++) {
        int from, to;
        std::cin >> to >> from;

        from--;
        to--;

        last_non_empty = to;
        holder[to].splice(holder[to].end(), holder[from]); /* O(1) */
    }

    for (const auto& word : holder[last_non_empty]) {
        std::cout << word;
    }

    std::cout << std::endl;
    return 0;
}

