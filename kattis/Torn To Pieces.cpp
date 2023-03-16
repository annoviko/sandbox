#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>


using graph = std::unordered_map<int, std::unordered_set<int>>;


class solution {
public:
    std::vector<int> get_path(graph& g, const int src, const int dst) {
        std::vector<int> parent(g.size(), -1);

        std::queue<int> queue;
        queue.push(src);
        parent[src] = -2;

        bool dst_found = false;

        while (!queue.empty()) {
            int cur = queue.front();
            queue.pop();

            for (int nei : g[cur]) {
                if (parent[nei] != -1) {
                    continue;
                }

                parent[nei] = cur;
                queue.push(nei);

                if (nei == dst) {
                    dst_found = true;
                    break;
                }
            }

            if (dst_found) {
                break;
            }
        }
        
        if (parent[dst] < 0) {
            return { };
        }

        std::vector<int> path;
        for (int i = dst; i >= 0; i = parent[i]) {
            path.push_back(i);
        }

        std::reverse(path.begin(), path.end());
        return path;
    }
};


int main() {
    int n = 0;
    std::string line;
    std::getline(std::cin, line);
    n = std::stoi(line);

    graph g(n);

    int id_counter = 0;
    std::unordered_map<std::string, int> name_dict;
    std::unordered_map<int, std::string> names;

    for (int i = 0; i < n; i++) {
        std::getline(std::cin, line);
        std::stringstream sstream(line);

        std::string from, to;
        sstream >> from;

        auto iter = name_dict.find(from);
        if (iter == name_dict.end()) {
            name_dict[from] = id_counter;
            names[id_counter] = from;
            id_counter++;
        }

        const int id_from = name_dict[from];

        while (sstream >> to) {
            auto iter = name_dict.find(to);
            if (iter == name_dict.end()) {
                name_dict[to] = id_counter;
                names[id_counter] = to;
                id_counter++;
            }

            const int id_to = name_dict[to];

            g[id_from].insert(id_to);
            g[id_to].insert(id_from);
        }
    }

    std::string src, dst;
    std::cin >> src >> dst;

    int id_src = name_dict[src];
    int id_dst = name_dict[dst];

    auto path = solution().get_path(g, id_src, id_dst);
    if (path.empty()) {
        std::cout << "no route found" << std::endl;
    }
    else {
        for (int i = 0; i < path.size(); i++) {
            std::cout << names[path[i]];
            if (i + 1 != path.size()) {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }

    return 0;
}
