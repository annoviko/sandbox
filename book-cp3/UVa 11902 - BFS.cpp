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
    void handle() {
        const auto dominations = get_dominations();

        for (std::size_t i = 0; i < m_graph.size(); i++) {
            std::cout << '+' << std::string(m_graph.size() + m_graph.size() - 1, '-') << '+' << std::endl;

            for (std::size_t j = 0; j < m_graph.size(); j++) {
                std::cout << '|';
                if (dominations[i][j]) {
                    std::cout << 'Y';
                }
                else {
                    std::cout << 'N';
                }
            }

            std::cout << '|' << std::endl;
        }

        std::cout << '+' << std::string(m_graph.size() + m_graph.size() - 1, '-') << '+' << std::endl;
    }

private:
    std::vector<std::vector<bool>> get_dominations() {
        std::vector<std::vector<bool>> result;

        std::vector<bool> root_domination = get_bfs_root_reachable();
        result.push_back(std::move(root_domination));

        for (std::size_t i = 1; i < m_graph.size(); i++) {
            std::vector<bool> node_domination = result.front();
            get_bsf_domination(i, node_domination);
            result.push_back(std::move(node_domination));
        }

        return result;
    }


    std::vector<bool> get_bfs_root_reachable() {
        std::vector<bool> visited = std::vector<bool>(m_graph.size(), false);

        std::queue<std::size_t> to_visit;
        to_visit.push(0);
        visited[0] = true;

        while (!to_visit.empty()) {
            std::size_t node = to_visit.front();
            to_visit.pop();

            for (std::size_t neighbor : m_graph[node]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    to_visit.push(neighbor);
                }
            }
        }

        return visited;
    }


    void get_bsf_domination(const std::size_t p_dominator, std::vector<bool>& p_domination) {
        std::vector<bool> node_domination(m_graph.size(), true);
        auto visited = std::vector<bool>(m_graph.size(), false);

        std::queue<std::size_t> to_visit;
        to_visit.push(0);
        visited[0] = true;

        while (!to_visit.empty()) {
            std::size_t reachable_node = to_visit.front();
            to_visit.pop();

            p_domination[reachable_node] = false;

            for (std::size_t neighbor : m_graph[reachable_node]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    if (p_dominator != neighbor) {
                        to_visit.push(neighbor);
                    }
                }
            }
        }
    }
};


graph read_graph() {
    std::size_t n = 0;
    std::cin >> n;

    graph result;

    for (std::size_t i = 0; i < n; i++) {
        result.push_back(std::vector<int>());

        for (std::size_t j = 0; j < n; j++) {
            std::size_t value = 0;
            std::cin >> value;

            if (value == 1) {
                result.back().push_back(j);
            }
        }
    }

    return result;
}


std::vector<graph> read_graphs() {
    std::size_t n = 0;
    std::cin >> n;

    std::vector<graph> result;
    for (std::size_t i = 0; i < n; i++) {
        result.push_back(read_graph());
    }

    return result;
}


int main() {
    auto graphs = read_graphs();

    for (std::size_t i = 0; i < graphs.size(); i++) {
        std::cout << "Case " << i + 1 << ":" << std::endl;
        Solution(graphs[i]).handle();
    }

    return 0;
}
