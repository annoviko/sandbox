#include <vector>
#include <unordered_map>


using graph = std::vector<std::vector<int>>;


const int UNVISITED = 0;
const int EXPLORED = 1;
const int VISITED = 2;


class safe_analyser {
private:
    const graph& g;
    std::vector<int> visited;

public:
    safe_analyser(const graph& _g) : g(_g) { }

public:
    bool analyse(const int id) {
        visited = std::vector<int>(g.size(), UNVISITED);
        return dfs(id);
    }

private:
    bool dfs(const int id) {
        visited[id] = EXPLORED;

        for (int nei : g[id]) {
            if (visited[nei] == EXPLORED) {
                return true;
            }
            else if (visited[nei] == UNVISITED) {
                if (dfs(nei)) {
                    return true;    /* no need to continue - cycle is found */
                }
            }
        }

        visited[id] = VISITED;
        return false;
    }
};


#include <iostream>
#include <string>

int main() {
    int n;
    std::cin >> n;

    graph g;

    std::unordered_map<int, std::string> id_to_name;
    std::unordered_map<std::string, int> name_to_id;

    for (int i = 0; i < n; i++) {
        std::string from, to;
        std::cin >> from >> to;

        if (name_to_id.find(from) == name_to_id.end()) {
            int id = g.size();
            name_to_id[from] = id;
            id_to_name[id] = from;
            g.push_back({ });
        }

        if (name_to_id.find(to) == name_to_id.end()) {
            int id = g.size();
            name_to_id[to] = g.size();
            id_to_name[id] = to;
            g.push_back({ });
        }

        int id_from = name_to_id[from];
        int id_to = name_to_id[to];

        g[id_from].push_back(id_to);
    }
    
    std::string name;
    while (std::cin >> name) {
        std::cout << name << " " << (safe_analyser(g).analyse(name_to_id[name]) ? "safe" : "trapped") << std::endl;
    }

    return 0;
}
