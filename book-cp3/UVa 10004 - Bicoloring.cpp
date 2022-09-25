#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>


using graph = std::vector<std::vector<int>>;


class Solution {
public:
    bool is_bicoloring(const graph& p_graph) {
        std::vector<int> colors(p_graph.size(), -1);

        bool is_good = true;

        std::queue<int> queue;
        queue.push(0);

        colors[0] = 0;

        while (!queue.empty() && is_good) {
            int cur_node = queue.front();
            queue.pop();

            auto& neighbors = p_graph[cur_node];
            for (int idx_nei : neighbors) {
                if (colors[idx_nei] == -1) {
                    colors[idx_nei] = 1 - colors[cur_node];
                    queue.push(idx_nei);
                }
                else if (colors[idx_nei] == colors[cur_node]) {
                    is_good = false;
                    break;
                }
            }
        }

        return is_good;
    }
};


int main() {
    std::vector<graph> cases;

    for (std::string line; std::getline(std::cin, line);) {
        if (line.empty()) {
            break;
        }

        int n = std::stoi(line);
        if (n == 0) {
            break;
        }

        std::getline(std::cin, line);
        int k = std::stoi(line);

        graph g = graph(n, std::vector<int>());
        for (int i = 0; i < k; i++) {
            std::getline(std::cin, line);
            std::stringstream stream(line);

            int from, to;
            stream >> from;
            stream >> to;

            g[from].push_back(to);
        }

        cases.push_back(g);
    }

    for (std::size_t i = 0; i < cases.size(); i++) {
        bool is_bicoloring = Solution().is_bicoloring(cases[i]);
        if (is_bicoloring) {
            std::cout << "BICOLORABLE." << std::endl;
        }
        else {
            std::cout << "NOT BICOLORABLE." << std::endl;
        }
    }

    return 0;
}