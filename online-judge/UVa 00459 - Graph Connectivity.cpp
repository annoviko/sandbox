#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <vector>
#include <unordered_map>


using graph = std::vector<std::vector<int>>;


class Solution {
private:
    graph m_graph;

public:
    Solution(const graph& p_graph) : m_graph(p_graph) { }


public:
    std::size_t count() {
        std::vector<bool> visited(m_graph.size(), false);
        std::size_t result = 0;

        for (std::size_t i = 0; i < m_graph.size(); i++) {
            if (!visited[i]) {
                bfs(i, visited);
                result++;
            }
        }

        return result;
    }

private:
    void bfs(const std::size_t p_node, std::vector<bool>& visited) {
        std::queue<int> to_visit;

        to_visit.push(p_node);
        visited[p_node] = true;

        while (!to_visit.empty()) {
            int cur_node = to_visit.front();
            to_visit.pop();

            for (const int neighbor : m_graph[cur_node]) {
                if (!visited[neighbor]) {
                    to_visit.push(neighbor);
                    visited[neighbor] = true;
                }
            }
        }
    }
};


graph read_graph() {
    std::string last_node;
    std::getline(std::cin, last_node);

    graph result;

    for (int i = 'A'; i <= last_node[0]; i++) {
        result.push_back(std::vector<int>());
    }

    for (std::string edge; std::getline(std::cin, edge) && !edge.empty();) {
        result[edge[0] - 'A'].push_back(edge[1] - 'A');
        result[edge[1] - 'A'].push_back(edge[0] - 'A');
    }

    return result;
}


int main() {
    std::string n_str;
    std::getline(std::cin, n_str);
    std::size_t n = std::stoull(n_str);

    std::string ignore;
    std::getline(std::cin, ignore);

    for (std::size_t i = 0; i < n; i++) {
        auto graph = read_graph();
        std::cout << Solution(graph).count() << std::endl;
        if (i != n - 1) {
            std::cout << std::endl;
        }
    }

    return 0;
}
