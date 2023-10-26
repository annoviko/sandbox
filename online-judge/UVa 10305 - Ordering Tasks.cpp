#include <vector>
#include <queue>


using graph = std::vector<std::vector<int>>;


class solution {
public:
    std::vector<int> get_order(const graph& g) {
        std::vector<int> result;

        std::vector<int> links(g.size(), 0);
        for (int from = 0; from < g.size(); from++) {
            for (int to : g[from]) {
                links[to]++;
            }
        }

        std::queue<int> queue;
        for (int i = 0; i < links.size(); i++) {
            if (links[i] == 0) {
                queue.push(i);
            }
        }

        while (!queue.empty()) {
            int node = queue.front();
            queue.pop();

            result.push_back(node);

            for (int nei : g[node]) {
                links[nei]--;
                if (links[nei] == 0) {
                    queue.push(nei);
                }
            }
        }

        return result;
    }
};


#include <iostream>


int main() {
    while (true) {
        int n, m;
        std::cin >> n >> m;

        if ((n == 0) && (m == 0)) {
            break;
        }

        graph g(n);
        for (int i = 0; i < m; i++) {
            int from, to;
            std::cin >> from >> to;

            g[from - 1].push_back(to - 1);
        }

        auto order = solution().get_order(g);
        for (int i = 0; i < order.size(); i++) {
            std::cout << order[i] + 1;
            if (i != order.size() - 1) {
                std::cout << ' ';
            }
        }
        std::cout << std::endl;
    }

    return 0;
}