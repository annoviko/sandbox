#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <unordered_map>


using graph = std::unordered_map<int, std::vector<int>>;


class comparator {
private:
    const std::vector<std::string>& names;

public:
    comparator(const std::vector<std::string>& p_names) : names(p_names) { }

    bool operator()(int lhs, int rhs) const {
        return names[lhs] > names[rhs];
    }
};


class solution {
private:
    graph& g;       /* from -> to */
    const std::vector<std::string>& names;

public:
    solution(graph& p_g, const std::vector<std::string>& p_names) : 
        g(p_g),
        names(p_names)
    { }

public:
    std::vector<int> get_order() {
        std::vector<int> inc_cons(names.size(), 0);
        for (const auto& pair : g) {
            int from = pair.first;
            for (int to : pair.second) {
                inc_cons[to]++;
            }
        }

        std::priority_queue<int, std::vector<int>, comparator> to_process((comparator(names)));

        for (int i = 0; i < inc_cons.size(); ++i) {
            if (inc_cons[i] == 0) {
                to_process.push(i);
            }
        }

        std::vector<int> order;

        while (!to_process.empty()) {
            int from = to_process.top();
            to_process.pop();

            order.push_back(from);

            for (int to : g[from]) {
                inc_cons[to]--;

                if (inc_cons[to] == 0) {
                    to_process.push(to);
                }
            }
        }

        if (order.size() != names.size()) {
            return { };
        }

        return order;
    }
};


int main() {
    int test_counter = 0;

    while (true) {
        std::unordered_map<std::string, int> name_to_id;
        std::vector<std::string> id_to_name;

        int free_id = 0;

        std::string temp;
        std::getline(std::cin, temp);

        int n = std::stoi(temp);
        if (n == 0) {
            break;
        }

        if (test_counter > 0) {
            std::cout << std::endl; /* blank line between tests */
        }

        graph g;

        for (int i = 0; i < n; i++) {
            std::string line;
            std::getline(std::cin, line);

            std::stringstream ss(line);
            std::string name;

            int to = -1;
            while (ss >> name) {
                int node_id = 0;

                auto iter = name_to_id.find(name);
                if (iter == name_to_id.cend()) {
                    name_to_id[name] = free_id;
                    id_to_name.push_back(name);

                    node_id = free_id;
                    free_id++;
                }
                else {
                    node_id = iter->second;
                }

                if (to == -1) {
                    to = node_id;
                }
                else {
                    g[node_id].push_back(to);
                }
            }
        }

        auto order = solution(g, id_to_name).get_order();
        if (order.empty()) {
            std::cout << "cannot be ordered" << std::endl;
        }
        else {
            for (int id : order) {
                std::cout << id_to_name[id] << std::endl;
            }
        }

        test_counter++;
    }

    return 0;
}
