#include <cctype>
#include <iostream>
#include <fstream>
#include <list>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>

using nodes = std::vector<std::string>;

using graph = std::unordered_map<std::string, nodes>;


struct context {
    std::unordered_set<std::string> visited;
    bool visit_twice = false;

#ifdef DEBUG_PATH
    std::list<std::string> path; // debug only
#endif
};


class path_finder {
private:
    graph m_graph;
    std::uint64_t m_pathes = 0;

    bool m_twice_visit_mode = false;

public:
    path_finder(const graph& p_graph, const bool p_visit_twice_mode) :
        m_graph(p_graph),
        m_twice_visit_mode(p_visit_twice_mode)
    { }


private:
    void traverse(const std::string& node, const context& p_context) {
        if (node == "end") {
#ifdef DEBUG_PATH
            for (const auto& node : p_context.path) {
                std::cout << node;
                if (node != "end") {
                    std::cout << " -> ";
                }
            }
            std::cout << std::endl;
#endif
            m_pathes++;
            return;
        }


        const nodes& directions = m_graph[node];

        for (const std::string& direction : directions) {
            context new_context = p_context;

            if (std::islower(direction[0])) { // small cave
                if (new_context.visited.count(direction) != 0) {
                    if (!m_twice_visit_mode) {
                        continue; // small cave can be visited only once
                    }
                    else {
                        if (new_context.visit_twice) {
                            continue;
                        }

                        new_context.visit_twice = true;
                    }
                }
            }

#ifdef DEBUG_PATH
            new_context.path.push_back(direction);
#endif
            new_context.visited.insert(direction);
            traverse(direction, new_context);
        }
    }

public:
    std::uint64_t count() {
        context new_context;
#ifdef DEBUG_PATH
        new_context.path.emplace_back("start");
#endif
        traverse("start", new_context);
        return m_pathes;
    }
};


graph read_input() {
    std::ifstream stream("input.txt");

    graph result;
    for (std::string str_row; std::getline(stream, str_row);) {
        const std::size_t position = str_row.find('-');

        const std::string from_node = str_row.substr(0, position);
        const std::string to_node = str_row.substr(position + 1);

        if (to_node != "start") {
            result[from_node].push_back(to_node);
        }

        if ((from_node != "start") && (to_node != "end")) {
            result[to_node].push_back(from_node);
        }
    }

    return result;
}


int main() {
    const graph input = read_input();

    std::cout << "The amount of paths visiting small caves only once: " << path_finder(input, false).count() << std::endl;
    std::cout << "The amount of paths visiting one small cave only twice: " << path_finder(input, true).count() << std::endl;

    return 0;
}