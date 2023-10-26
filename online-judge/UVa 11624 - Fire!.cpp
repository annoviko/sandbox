#include <queue>
#include <vector>
#include <string>

using maze = std::vector<std::string>;


struct position {
    int row = 0;
    int col = 0;

    position(int p_row, int p_col) :
        row(p_row), col(p_col)
    { }
};


class solution {
private:
    maze m_maze;

    std::queue<position> m_fire;
    std::queue<position> m_player;
    std::vector<std::vector<bool>> m_visited;

public:
    solution(const maze& p_maze) :
        m_maze(p_maze),
        m_visited(p_maze.size(), std::vector<bool>(p_maze[0].size(), false))
    {
        for (int i = 0; i < m_maze.size(); i++) {
            for (int j = 0; j < m_maze[i].size(); j++) {
                if (m_maze[i][j] == 'F') {
                    m_fire.push(position(i, j));
                }
                else if (m_maze[i][j] == 'J') {
                    m_player.push(position(i, j));
                    m_maze[i][j] = '.';
                    m_visited[i][j] = true;
                }
            }
        }
    }

public:
    int escape() {
        int iteration = 0;
        
        while (!m_player.empty()) {
            iteration++;

            spread_fire();
            std::queue<position> bfs_player;

            while (!m_player.empty()) {
                auto cur = m_player.front();
                m_player.pop();

                for (auto nei : get_neighbors(cur.row, cur.col)) {
                    if (is_exit(nei.row, nei.col)) {
                        return iteration;
                    }

                    if (m_visited[nei.row][nei.col]) {
                        continue;
                    }

                    if (is_spot(nei.row, nei.col)) {
                        bfs_player.push(nei);
                        m_visited[nei.row][nei.col] = true;
                    }
                }
            }

            m_player = std::move(bfs_player);
        }

        return -1;
    }

private:
    bool is_exit(int row, int col) {
        return (row < 0 || row >= m_maze.size()) || 
            (col < 0 || col >= m_maze[0].size());
    }


    bool is_spot(int row, int col) {
        if (row < 0 || row >= m_maze.size()) {
            return false;
        }

        if (col < 0 || col >= m_maze[0].size()) {
            return false;
        }

        return m_maze[row][col] == '.';
    }

    std::vector<position> get_neighbors(int row, int col) {
        std::vector<position> result;

        result.emplace_back(row - 1, col);
        result.emplace_back(row + 1, col);
        result.emplace_back(row, col - 1);
        result.emplace_back(row, col + 1);

        return result;
    }

    void spread_fire() {
        std::queue<position> next_bfs_fire;

        while (!m_fire.empty()) {
            auto cur = m_fire.front();
            m_fire.pop();

            for (auto nei : get_neighbors(cur.row, cur.col)) {
                if (is_spot(nei.row, nei.col)) {
                    next_bfs_fire.push(nei);
                    m_maze[nei.row][nei.col] = 'F';
                }
            }
        }

        m_fire = std::move(next_bfs_fire);
    }
};


#include <iostream>
#include <sstream>

int main() {
    std::string line;
    std::getline(std::cin, line);

    int n = std::stoi(line);
    for (int i = 0; i < n; i++) {
        std::getline(std::cin, line);
        std::stringstream stream(line);

        int r, c;
        stream >> r >> c;

        maze m;
        for (int j = 0; j < r; j++) {
            std::getline(std::cin, line);
            m.push_back(line);
        }

        int steps = solution(m).escape();
        if (steps == -1) {
            std::cout << "IMPOSSIBLE" << std::endl;
        }
        else {
            std::cout << steps << std::endl;
        }
    }

    return 0;
}