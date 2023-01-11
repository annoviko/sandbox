#include <list>
#include <vector>
#include <queue>


using graph = std::vector<std::vector<int>>;


class solution {
public:
    std::list<int> find_shortest_path(const graph& g, const int source, const int destination) {
        if (source == destination) {
            return std::list<int>(1, source);
        }

        std::vector<int> parents(g.size(), -1);

        std::queue<int> to_process;
        to_process.push(source);

        std::vector<bool> visited(g.size(), false);
        visited[source] = true;

        while (!to_process.empty()) {
            int cur = to_process.front();
            to_process.pop();

            for (int nei : g[cur]) {
                if (visited[nei]) {
                    continue;
                }

                parents[nei] = cur;
                visited[nei] = true;
                to_process.push(nei);

                if (nei == destination) {
                    std::list<int> result;
                    for (int idx = nei; idx != -1; idx = parents[idx]) {
                        result.push_front(idx);
                    }

                    return result;
                }
            }
        }

        return std::list<int>();
    }
};


#include <iostream>
#include <string>
#include <sstream>


int main() {
    for (std::string line; std::getline(std::cin, line); ) {
        if (line.empty()) {
            return 0;
        }

        std::cout << "-----" << std::endl;

        int n = std::stoi(line);

        graph g(n);
        for (int i = 0; i < n; i++) {
            std::getline(std::cin, line);
            std::vector<int> values;

            std::string str_val;
            for (int i = 0; i < line.size(); i++) {
                if (std::isdigit(line[i])) {
                    str_val.push_back(line[i]);
                }
                else if (!str_val.empty()) {
                    values.push_back(std::stoi(str_val));
                    str_val.clear();
                }
            }

            if (!str_val.empty()) {
                values.push_back(std::stoi(str_val));
            }

            const int from = values[0];
            for (int i = 1; i < values.size(); i++) {
                const int to = values[i];
                g[from - 1].push_back(to - 1);
            }
        }

        std::getline(std::cin, line);
        int m = std::stoi(line);

        for (int i = 0; i < m; i++) {
            std::getline(std::cin, line);
            std::stringstream stream(line);

            int from, to;
            stream >> from >> to;

            from--;
            to--;

            std::list<int> path = solution().find_shortest_path(g, from, to);
            if (path.empty()) {
                std::cout << "connection impossible" << std::endl;
            }
            else {
                while (!path.empty()) {
                    std::cout << (path.front() + 1);
                    path.pop_front();

                    if (path.size() > 0) {
                        std::cout << ' ';
                    }
                }
                std::cout << std::endl;
            }
        }
    }

    return 0;
}