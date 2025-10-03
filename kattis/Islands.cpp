#include <vector>
#include <iostream>
#include <queue>
#include <string>


struct position {
    int r;
    int c;
};


void bfs(int r, int c, const std::vector<std::string>& field, std::vector<std::vector<bool>>& visited) {
    std::queue<position> to_proc;
    to_proc.push({ r, c });

    visited[r][c] = true;

    while (!to_proc.empty()) {
        auto cur = to_proc.front();
        to_proc.pop();

        std::vector<position> neis = {
            { cur.r + 1, cur.c },
            { cur.r - 1, cur.c },
            { cur.r, cur.c + 1 },
            { cur.r, cur.c - 1 }
        };

        for (const auto& nei : neis) {
            if (nei.r < 0 || nei.r >= field.size() || nei.c < 0 || nei.c >= field[0].size()) {
                continue;
            }

            if (field[nei.r][nei.c] == 'W') {
                continue;
            }

            if (visited[nei.r][nei.c]) {
                continue;
            }

            visited[nei.r][nei.c] = true;
            to_proc.push(nei);
        }
    }
}


int main() {
    int rows, cols;
    std::cin >> rows >> cols;

    std::vector<std::string> field(rows);
    for (int i = 0; i < rows; i++) {
        std::cin >> field[i];
    }

    int counter = 0;
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (visited[r][c]) {
                continue;
            }

            switch (field[r][c]) {
            case 'L':
                bfs(r, c, field, visited);
                counter++;
                break;
            }
        }
    }

    std::cout << counter << std::endl;
    return 0;
}