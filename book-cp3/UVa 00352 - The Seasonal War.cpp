#include <iostream>
#include <queue>
#include <vector>


using graph = std::vector<std::vector<int>>;


struct position {
    int row;
    int col;

    position(int p_row, int p_col) :
        row(p_row),
        col(p_col)
    { }
};


class Solution {
private:
    graph m_graph;
    std::vector<std::vector<bool>> m_visited;

public:
    Solution(const graph& p_graph) :
        m_graph(p_graph),
        m_visited(p_graph.size(), std::vector<bool>(p_graph[0].size(), false))
    { }

public:
    int count_war_eagls() {
        int result = 0;

        for (int i = 0; i < (int) m_graph.size(); i++) {
            for (int j = 0; j < (int) m_graph[0].size(); j++) {
                if (!m_visited[i][j] && m_graph[i][j] == 1) {
                    result++;
                    bfs(i, j);
                }
            }
        }

        return result;
    }

private:
    void bfs(int i, int j) {
        std::queue<position> queue;
        queue.push(position(i, j));

        m_visited[i][j] = true;

        while(!queue.empty()) {
            auto cur_node = queue.front();
            queue.pop();

            auto neighbors = get_neighbors(cur_node);
            for (auto & nei_pos : neighbors) {
                if (m_graph[nei_pos.row][nei_pos.col] == 1 && !m_visited[nei_pos.row][nei_pos.col]) {
                    m_visited[nei_pos.row][nei_pos.col] = true;
                    queue.push(nei_pos);
                }
            }
        }
    }

    std::vector<position> get_neighbors(const position & p_pos) {
        std::vector<position> result;

        if (p_pos.row > 0) {
            result.push_back(position(p_pos.row - 1, p_pos.col)); // upper
        }

        if ((p_pos.row > 0) && (p_pos.col < m_graph[0].size() - 1)) {
            result.push_back(position(p_pos.row - 1, p_pos.col + 1)); // upper right
        }

        if (p_pos.col < m_graph[0].size() - 1) {
            result.push_back(position(p_pos.row, p_pos.col + 1)); // right
        }

        if ((p_pos.row < m_graph.size() - 1) && (p_pos.col < m_graph[0].size() - 1)) {
            result.push_back(position(p_pos.row + 1, p_pos.col + 1)); // lower - right
        }

        if (p_pos.row < m_graph.size() - 1) {
            result.push_back(position(p_pos.row + 1, p_pos.col)); // lower
        }

        if ((p_pos.row < m_graph.size() - 1) && (p_pos.col > 0)) {
            result.push_back(position(p_pos.row + 1, p_pos.col - 1)); // lower - left
        }

        if (p_pos.col > 0) {
            result.push_back(position(p_pos.row, p_pos.col - 1)); // left
        }

        if ((p_pos.row > 0) && (p_pos.col > 0)) {
            result.push_back(position(p_pos.row - 1, p_pos.col - 1)); // left - upper
        }

        return result;
    }
};


#include <string>
#include <sstream>

int main() {
    int counter = 0;
    for (std::string line; std::getline(std::cin, line);) {
        if (line.empty()) {
            return 0;
        }

        std::stringstream stream(line);
        int matrix_size;
        stream >> matrix_size;

        graph g;
        for (int i = 0; i < matrix_size; i++) {
            std::getline(std::cin, line);

            std::vector<int> row;
            for (const char elem : line) {
                row.push_back(elem - '0');
            }

            g.push_back(row);
        }

        counter++;
        const int result = Solution(g).count_war_eagls();

        std::cout << "Image number " << counter << " contains " << result << " war eagles." << std::endl;
    }

    return 0;
}
