#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>


using graph = std::vector<std::vector<int>>;


class Solution {
private:
    graph m_graph;
    std::vector<bool> m_visited;

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

        std::vector<bool> root_domination(m_graph.size(), false);
        get_reachable(0, root_domination);

        result.push_back(std::move(root_domination));

        for (std::size_t i = 1; i < m_graph.size(); i++) {
            m_visited = std::vector<bool>(m_graph.size(), false);

            std::vector<bool> node_domination = result.front();
            get_domination(0, i, node_domination);
            
            result.push_back(std::move(node_domination));
        }

        return result;
    }

   void get_reachable(std::size_t p_node, std::vector<bool>& p_reachability) {
       if (p_reachability[p_node]) {
           return; /* already visited */
       }

       p_reachability[p_node] = true;

       for (std::size_t neighbor : m_graph[p_node]) {
           get_reachable(neighbor, p_reachability);
       }
   }

   void get_domination(std::size_t p_node, std::size_t p_excluded, std::vector<bool>& p_root_domination) {
       if (p_node == p_excluded) {
           return;
       }

       if (m_visited[p_node]) {
           return; /* already visited */
       }

       m_visited[p_node] = true;

       p_root_domination[p_node] = false; // we can reach this node anyway
       for (std::size_t neighbor : m_graph[p_node]) {
           get_domination(neighbor, p_excluded, p_root_domination);
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
