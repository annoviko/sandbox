#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>


using graph = std::vector<std::vector<bool>>;


struct position {
    int row = 0;
    int col = 0;

    position() { }
    position(int r, int c) : row(r), col(c) { }
};


class Solution {
private:
    graph m_graph;

public:
    Solution(const graph& p_graph) : m_graph(p_graph) { }


public:
    std::size_t bfs(const position& p_initial) {
        if (!m_graph[p_initial.row][p_initial.col]) {
            return 0;
        }

        std::size_t result = 0;
        std::vector<std::vector<bool>> visited(m_graph.size(), std::vector<bool>(m_graph[0].size(), false));

        std::queue<position> queue;
        queue.push(p_initial);

        visited[p_initial.row][p_initial.col] = true;

        while (!queue.empty()) {
            position cur = queue.front();
            queue.pop();

            result++;
            const auto neighbors = get_neighbors(cur);

            for (const auto node : neighbors) {
                if (!visited[node.row][node.col] && m_graph[node.row][node.col]) {
                    visited[node.row][node.col] = true;
                    queue.push(node);
                }
            }
        }

        return result;
    }

    std::vector<position> get_neighbors(const position& p_pos) {
        std::vector<position> result;
        if (p_pos.row - 1 >= 0) {
            result.push_back(position(p_pos.row - 1, p_pos.col));
        }
        
        if (p_pos.row + 1 < (int) m_graph.size()) {
            result.push_back(position(p_pos.row + 1, p_pos.col));
        }
        
        if (p_pos.col - 1 >= 0) {
            result.push_back(position(p_pos.row, p_pos.col - 1));
        }
        
        if (p_pos.col + 1 < (int) m_graph[0].size()) {
            result.push_back(position(p_pos.row, p_pos.col + 1));
        }

        if (p_pos.row - 1 >= 0 && p_pos.col - 1 >= 0) {
            result.push_back(position(p_pos.row - 1, p_pos.col - 1));
        }

        if (p_pos.row - 1 >= 0 && p_pos.col + 1 < (int)m_graph[0].size()) {
            result.push_back(position(p_pos.row - 1, p_pos.col + 1));
        }

        if (p_pos.row + 1 < (int)m_graph.size() && p_pos.col - 1 >= 0) {
            result.push_back(position(p_pos.row + 1, p_pos.col - 1));
        }

        if (p_pos.row + 1 < (int)m_graph.size() && p_pos.col + 1 < (int)m_graph[0].size()) {
            result.push_back(position(p_pos.row + 1, p_pos.col + 1));
        }

        return result;
    }
};


struct task {
    graph map;
    std::vector<position> poses;
};


struct input {
    std::size_t n;
    std::vector<task> cases;
};


input read_input() {
    input result;

    std::string n_str;
    std::getline(std::cin, n_str);
    result.n = std::stoull(n_str); // cases

    std::string ignore;
    std::getline(std::cin, ignore);

    for (std::size_t i = 0; i < result.n; i++) {
        result.cases.push_back({ { }, { } });

        std::string str;
        while (true) {
            std::getline(std::cin, str);
            if (str[0] != 'L' && str[0] != 'W') {
                break; // graph ended
            }

            std::vector<bool> row;
            for (auto sym : str) {
                row.push_back(sym == 'W');
            }

            result.cases.back().map.push_back(row);
        }

        // initial points
        while (!str.empty()) {
            std::istringstream stream(str);

            position pos;
            stream >> pos.row;
            stream >> pos.col;

            pos.row--;
            pos.col--;

            result.cases.back().poses.push_back(pos);
            std::getline(std::cin, str);
        }
    }

    return result;
}


int main() {
    auto in = read_input();
    for (std::size_t i = 0; i < in.n; i++) {
        task& t = in.cases[i];
        Solution s = Solution(t.map);

        for (std::size_t j = 0; j < t.poses.size(); j++) {
            std::cout << s.bfs(t.poses[j]) << std::endl;
        }

        if (i + 1 != in.n) {
            std::cout << std::endl;
        }
    }

    return 0;
}
