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
    std::vector<std::string> get_order(const graph& p_graph, const std::vector<std::string> p_names) {
        std::vector<int> counters(p_names.size(), 0);

        for (std::size_t i = 0; i < p_graph.size(); i++) {
            for (const std::size_t idx_neighbor : p_graph[i]) {
                counters[idx_neighbor]++;
            }
        }

        std::vector<std::string> order;

        std::priority_queue<int, std::vector<int>, std::greater<int> > queue;
        for (std::size_t i = 0; i < counters.size(); i++) {
            if (counters[i] == 0) {
                queue.push(i);
            }
        }

        while (!queue.empty()) {
            int cur = queue.top();
            queue.pop();

            order.push_back(p_names[cur]);

            const auto& neighbors = p_graph[cur];
            for (const auto idx_neigh : neighbors) {
                counters[idx_neigh]--;
                if (counters[idx_neigh] == 0) {
                    queue.push(idx_neigh);
                }
            }
        }

        return order;
    }
};


struct task {
    std::vector<std::string> names;
    graph connections;
};


int main() {
    std::vector<task> cases;

    for (std::string line; std::getline(std::cin, line);) {
        if (line.empty()) {
            break;
        }

        int n = std::stoi(line);

        std::vector<std::string> names;
        std::unordered_map<std::string, int> name_to_idx;

        for (std::size_t i = 0; i < n; i++) {
            std::getline(std::cin, line);

            names.push_back(line);
            name_to_idx[line] = i;
        }

        std::getline(std::cin, line);
        n = std::stoi(line);

        graph connections(names.size(), std::vector<int>());
        for (std::size_t i = 0; i < n; i++) {
            std::getline(std::cin, line);
            std::stringstream stream(line);

            std::string first, second;
            stream >> first;
            stream >> second;

            connections[name_to_idx[first]].push_back(name_to_idx[second]);
        }

        task t;
        t.connections = std::move(connections);
        t.names = std::move(names);

        cases.push_back(t);

        std::getline(std::cin, line); // ignore
    }

    for (std::size_t i = 0; i < cases.size(); i++) {
        auto order = Solution().get_order(cases[i].connections, cases[i].names);

        std::cout << "Case #" << i + 1 << ": Dilbert should drink beverages in this order:";
        for (const auto& drink : order) {
            std::cout << " " << drink;
        }

        std::cout << "." << std::endl << std::endl;
    }

    return 0;
}