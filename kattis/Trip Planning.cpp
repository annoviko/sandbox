#include <iostream>
#include <vector>
#include <unordered_map>


int main() {
    int n, m;
    std::cin >> n >> m;

    std::vector<std::unordered_map<int, int>> graph(n);
    for (int i = 0; i < m; i++) {
        int from, to;
        std::cin >> from >> to;

        from--;
        to--;

        graph[from].insert({ to, i + 1 });
        graph[to].insert({ from, i + 1 });
    }

    std::vector<int> stops;
    for (int i = 0; i < n - 1; i++) {
        auto iter = graph[i].find(i + 1);
        if (iter == graph[i].cend()) {
            std::cout << "impossible" << std::endl;
            return 0;
        }

        stops.push_back(iter->second);
    }

    auto iter_back_home = graph.back().find(0);
    if (iter_back_home == graph.back().cend()) {
        std::cout << "impossible" << std::endl;
    }
    else {
        stops.push_back(iter_back_home->second);
        for (auto stop : stops) {
            std::cout << stop << std::endl;
        }
    }

    return 0;
}