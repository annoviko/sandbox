#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <queue>


struct position {
    int x;
    int y;
    int z;
};


struct block {
public:
    int id;
    position begin;
    position end;
};


class solution {
private:
    std::vector<block> blocks;
    std::vector<std::vector<std::vector<int>>> field;

    int x_max = 0;
    int y_max = 0;
    int z_max = 0;

    std::unordered_map<int, std::vector<int>> g;    /* graph [id] -> [supporters] */

public:
    solution(const std::vector<block>& b) :
        blocks(b)
    {
        initialize_field();
        stabilize();
        place_blocks_on_field();
    }

public:
    int count_disintegratable_blocks() {
        std::vector<std::vector<int>> gf(blocks.size());        /* graph [id] -> [who is supported by id] */
        std::unordered_map<int, std::unordered_set<int>> gl;    /* graph [id] -> [who supports id] */

        std::unordered_set<int> visited;
        for (int z = z_max; z >= 0; z--) {
            for (int x = 0; x <= x_max; x++) {
                for (int y = 0; y <= y_max; y++) {
                    if (field[x][y][z] == -1) {
                        continue;
                    }

                    if (visited.count(field[x][y][z]) > 0) {
                        continue;
                    }

                    visited.insert(field[x][y][z]);

                    auto supporters = find_supporters(field[x][y][z], x, y, z);

                    gl[field[x][y][z]] = supporters;
                    for (int id_supporter : supporters) {
                        gf[id_supporter].push_back(field[x][y][z]);
                    }
                }
            }
        }

        int counter = 0;
        for (int id = 0; id < blocks.size(); id++) {
            if (gf[id].size() == 0) {
                counter++;  /* a block does not support anyone - can be removed */
                continue;
            }

            /* check if supported blocks has another blocks */
            bool can_be_removed = true;
            for (int above_block_id : gf[id]) {
                if (gl[above_block_id].size() == 1) {
                    can_be_removed = false;  /* a block above has only one supporter - can not be removed */
                    break;
                }
            }

            /* can be removed - blocks above have more than one supporter */
            if (can_be_removed) {
                counter++;
            }
        }

        return counter;
    }

private:
    void stabilize() {
        std::sort(blocks.begin(), blocks.end(), [](const block& l, const block& r) {
            return l.begin.z < r.begin.z;
        });

        std::vector<std::vector<int>> surface(x_max + 1, std::vector<int>(y_max + 1, 0));

        for (auto& b : blocks) {
            int max_z_drop = b.begin.z;

            for (int x = b.begin.x; x <= b.end.x; x++) {
                for (int y = b.begin.y; y <= b.end.y; y++) {
                    max_z_drop = std::min(b.begin.z - surface[x][y], max_z_drop);
                }
            }

            int new_z = b.begin.z - max_z_drop;

            b.end.z = new_z + (b.end.z - b.begin.z);
            b.begin.z = new_z;

            for (int x = b.begin.x; x <= b.end.x; x++) {
                for (int y = b.begin.y; y <= b.end.y; y++) {
                    surface[x][y] = b.end.z + 1;
                }
            }
        }
    }

    std::unordered_set<int> find_supporters(int id, int x, int y, int z) {
        std::queue<position> to_proc;
        to_proc.push({ x, y, z });

        std::vector<std::vector<std::vector<bool>>> visited = std::vector<std::vector<std::vector<bool>>>(x_max + 1, std::vector<std::vector<bool>>(y_max + 1, std::vector<bool>(z_max + 1, false)));
        visited[x][y][z] = true;

        std::unordered_set<int> supporters;
        while (!to_proc.empty()) {
            position cur = to_proc.front();
            to_proc.pop();

            std::vector<position> neis = {
                { cur.x + 1, cur.y, cur.z },
                { cur.x - 1, cur.y, cur.z },
                { cur.x, cur.y + 1, cur.z },
                { cur.x, cur.y - 1, cur.z },
                { cur.x, cur.y, cur.z + 1 },
                { cur.x, cur.y, cur.z - 1 }
            };

            for (const auto& nei : neis) {
                if (nei.x > x_max || nei.x < 0 ||
                    nei.y > y_max || nei.y < 0 ||
                    nei.z > z_max || nei.z < 0) {
                    continue;
                }

                if (visited[nei.x][nei.y][nei.z]) {
                    continue;
                }

                visited[nei.x][nei.y][nei.z] = true;

                const int nei_id = field[nei.x][nei.y][nei.z];

                if (nei_id == -1) {
                    continue;
                }

                if (nei_id != id && nei.z < cur.z) {
                    if (field[nei.x][nei.y][nei.z + 1] == id) {
                        supporters.insert(nei_id);  /* the neighbor is supporter */
                    }

                    continue;
                }

                to_proc.push(nei);
            }
        }

        return supporters;
    }

    void initialize_field() {
        for (const auto& b : blocks) {
            x_max = std::max(x_max, b.end.x);
            y_max = std::max(y_max, b.end.y);
            z_max = std::max(z_max, b.end.z);
        }

        field = std::vector<std::vector<std::vector<int>>>(x_max + 1, std::vector <std::vector<int>>(y_max + 1, std::vector<int>(z_max + 1, -1)));
    }

    void place_block_on_field(const block& b) {
        for (int x = b.begin.x; x <= b.end.x; x++) {
            for (int y = b.begin.y; y <= b.end.y; y++) {
                for (int z = b.begin.z; z <= b.end.z; z++) {
                    if (field[x][y][z] != -1) {
                        throw std::exception("intersection between blocks detected - incorrect algorithm.");
                    }

                    field[x][y][z] = b.id;
                }
            }
        }
    }

    void place_blocks_on_field() {
        for (const auto& b : blocks) {
            place_block_on_field(b);
        }
    }
};


std::vector<block> read_input() {
    std::ifstream stream("input.txt");

    std::vector<block> blocks;

    int id = 0;
    for (std::string line; std::getline(stream, line);) {
        std::stringstream ss(line);

        char ignore;
        block b;

        b.id = id;
        ss >> b.begin.x >> ignore >> b.begin.y >> ignore >> b.begin.z >> ignore >> b.end.x >> ignore >> b.end.y >> ignore >> b.end.z;

        blocks.push_back(b);
        id++;
    }

    return blocks;
}


int main() {
    std::vector<block> input = read_input();

    int count = solution(input).count_disintegratable_blocks();

    std::cout << "Bricks could be safely chosen as the one to get disintegrated: " << count << std::endl;

    return 0;
}
