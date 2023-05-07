#include <queue>
#include <vector>
#include <unordered_map>


using graph = std::vector<std::vector<int>>;


class solution {
public:
    std::vector<int> recompile(const graph& g, const int id_file) {
        std::vector<int> independent(g.size(), false);

        std::vector<int> incoming(g.size(), 0);
        for (int from = 0; from < g.size(); from++) {
            for (int to : g[from]) {
                incoming[to]++;
            }
        }

        std::queue<int> to_process;
        for (int i = 0; i < incoming.size(); i++) {
            if (incoming[i] == 0) {
                to_process.push(i);
                independent[i] = true;
            }
        }

        bool file_found = false;
        while (!to_process.empty()) {
            int id = to_process.front();
            to_process.pop();

            if (id == id_file) {
                continue;   /* do not touch file to recompile and as a result every file that depends on it */
            }

            for (int nei : g[id]) {
                incoming[nei]--;
                if (incoming[nei] == 0) {
                    independent[nei] = true;
                    to_process.push(nei);
                }
            }
        }

        /* now we have graph where nodes depend on id_file - run topological sort again */
        std::vector<int> result;

        to_process.push(id_file);
        while (!to_process.empty()) {
            int id = to_process.front();
            to_process.pop();

            result.push_back(id);
            for (int nei : g[id]) {
                if (independent[nei]) {
                    continue;
                }

                incoming[nei]--;
                if (incoming[nei] == 0) {
                    independent[nei] = true;    /* can be compiled */
                    to_process.push(nei);
                }
            }
        }

        return result;
    }
};


#include <iostream>
#include <string>
#include <sstream>

int main() {
    std::string line;
    std::getline(std::cin, line);

    int n = std::stoi(line);

    std::unordered_map<int, std::string> id_to_name;
    std::unordered_map<std::string, int> name_to_id;

    graph g(n);

    for (int i = 0; i < n; i++) {
        std::string rule;
        std::getline(std::cin, rule);

        std::stringstream stream(rule);
        std::string file;

        stream >> file;
        file.pop_back();
        if (name_to_id.find(file) == name_to_id.end()) {
            int id = name_to_id.size();
            name_to_id[file] = id;
            id_to_name[id] = file;
        }

        int to = name_to_id[file];

        while (stream >> file) {
            if (name_to_id.find(file) == name_to_id.end()) {
                int id = name_to_id.size();
                name_to_id[file] = id;
                id_to_name[id] = file;
            }

            int from = name_to_id[file];
            g[from].push_back(to);
        }
    }

    std::string file_to_recompile;
    std::cin >> file_to_recompile;
    int id_file = name_to_id[file_to_recompile];

    auto order = solution().recompile(g, id_file);

    for (int id : order) {
        std::cout << id_to_name[id] << std::endl;
    }

    return 0;
}
