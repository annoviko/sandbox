#if 1

#include <iostream>
#include <vector>


int main() {
    int n, m;
    std::cin >> n >> m;

    std::vector<int> stops(n);
    int routes = 0;
    for (int i = 0; i < m; ++i) {
        int from, to;
        std::cin >> from >> to;

        from--;
        to--;

        const int lnode = std::min(from, to);
        const int rnode = std::max(from, to);
        const int order_diff = rnode - lnode;

        if (lnode == 0 && rnode == n - 1) {
            stops[rnode] = i + 1;
            routes++;
        }

        if (order_diff == 1) {
            stops[lnode] = i + 1;
            routes++;
        }
    }

    if (routes != n) {
        std::cout << "impossible" << '\n';
    }
    else {
        for (const auto stop : stops) {
            std::cout << stop << '\n';
        }
    }

    return 0;
}

#else
	
/* acceptable, but slower - builds graph and checks reachability of the next node */

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

#endif