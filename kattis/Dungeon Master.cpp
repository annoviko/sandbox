#include <iostream>
#include <string>
#include <vector>
#include <queue>


struct position {
    int r = -1;
    int c = -1;
    int z = -1;
};


using vec2d = std::vector<std::string>;
using vec3d = std::vector<vec2d>;


int main() {
    while (true) {
        int l, r, c;
        std::cin >> l >> r >> c;

        if (l == 0 && r == 0 && c == 0) {
            break;
        }

        position start, exit;

        vec3d field;
        for (int i = 0; i < l; i++) {
            vec2d layer;
            for (int j = 0; j < r; j++) {
                std::string row;
                for (int k = 0; k < c; k++) {
                    char cell;
                    std::cin >> cell;

                    switch (cell) {
                    case 'S':
                        start = { j, k, i };
                        break;
                    case 'E':
                        exit = { j, k, i };
                        break;
                    }

                    row.push_back(cell);
                }
                layer.push_back(row);
            }

            field.push_back(layer);
        }

        std::vector<std::vector<std::vector<bool>>> visited(l, std::vector<std::vector<bool>>(r, std::vector<bool>(c, false)));

        std::queue<position> to_proc;
        to_proc.push(start);
        visited[start.z][start.r][start.c] = true;

        int counter = 0;
        bool found = false;

        std::queue<position> to_proc_next;
        while (!to_proc.empty()) {
            position cur = to_proc.front();
            to_proc.pop();

            if (field[cur.z][cur.r][cur.c] == 'E') {
                found = true;
                break;
            }

            std::vector<position> candidates = {
                { cur.r + 1, cur.c, cur.z },
                { cur.r - 1, cur.c, cur.z },
                { cur.r, cur.c + 1, cur.z },
                { cur.r, cur.c - 1, cur.z },
                { cur.r, cur.c, cur.z + 1 },
                { cur.r, cur.c, cur.z - 1 },
            };

            for (const auto& pos : candidates) {
                if (pos.r < 0 || pos.r >= r || pos.c < 0 || pos.c >= c || pos.z < 0 || pos.z >= l) {
                    continue;
                }

                const char cell = field[pos.z][pos.r][pos.c];
                if (cell != '.' && cell != 'E') {
                    continue;
                }

                if (visited[pos.z][pos.r][pos.c]) {
                    continue;
                }

                visited[pos.z][pos.r][pos.c] = true;
                to_proc_next.push(pos);
            }

            if (to_proc.empty()) {
                to_proc = std::move(to_proc_next);
                counter++;
            }
        }

        if (found) {
            std::cout << "Escaped in " << counter << " minute(s)." << std::endl;
        }
        else {
            std::cout << "Trapped!" << std::endl;
        }
    }

    return 0;
}