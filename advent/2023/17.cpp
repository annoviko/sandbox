#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>


std::vector<std::vector<int>> read_input() {
    std::fstream stream("input.txt");

    std::vector<std::vector<int>> result;
    for (std::string line; std::getline(stream, line);) {
        std::vector<int> row;
        for (int i = 0; i < line.size(); i++) {
            row.push_back(line[i] - '0');
        }

        result.push_back(row);
    }

    return result;
}


struct state {
    int weight = 0;
    int row = 0;
    int col = 0;
    int dir_row = 0;
    int dir_col = 0;
    int dir_steps = 0;

    bool operator<(const state& other) const {
        if (weight < other.weight) { return true; }
        else if (weight > other.weight) { return false; }

        if (row < other.row) { return true; }
        else if (row > other.row) { return false; }

        if (col < other.col) { return true; }
        else if (col > other.col) { return false; }

        if (dir_row < other.dir_row) { return true; }
        else if (dir_row > other.dir_row) { return false; }

        if (dir_col < other.dir_col) { return true; }
        else if (dir_col > other.dir_col) { return false; }

        if (dir_steps < other.dir_steps) { return true; }
        return false;
    }

    bool operator==(const state& other) const {
        return weight == other.weight && row == other.row && col == other.col && 
            dir_row == other.dir_row && dir_col == other.dir_col && dir_steps == other.dir_steps;
    }
};


#define dict_t std::unordered_map


class path_finder {
private:
    /*    [row]       [col]    [dir_row]    [dir_col]    [dir_steps]->[int]    */
    dict_t<int, dict_t<int, dict_t<int, dict_t<int, dict_t<int, int>>>>> price;

public:
    int analyser(const std::vector<std::vector<int>>& g, bool elves_something_changed) {
        std::set<state> to_process;

        to_process.insert(state{});

        while (!to_process.empty()) {
            const state cur = *(to_process.begin());
            to_process.erase(to_process.begin());

            if (cur.row < 0 || cur.row >= g.size() || cur.col < 0 || cur.col >= g[0].size()) {
                continue;   /* outside */
            }

            if ( 
                (!elves_something_changed && cur.row == g.size() - 1 && cur.col == g[0].size() - 1) ||  /* part 1 */
                (elves_something_changed && cur.row == g.size() - 1 && cur.col == g[0].size() - 1 && cur.dir_steps >= 4)    /* part 2 */
               ) 
            {
                return cur.weight;
            }

            if (
                (!elves_something_changed && cur.dir_steps < 3 && !(cur.dir_row == 0 && cur.dir_col == 0)) ||   /* part 1 */
                (elves_something_changed && cur.dir_steps < 10 && !(cur.dir_row == 0 && cur.dir_col == 0))  /* part 2 */
               ) 
            {
                state next;

                next.row = cur.row + cur.dir_row;
                next.col = cur.col + cur.dir_col;
                next.dir_row = cur.dir_row;
                next.dir_col = cur.dir_col;
                next.dir_steps = cur.dir_steps + 1;

                if (next.row >= 0 && next.row < g.size() && next.col >= 0 && next.col < g[0].size()) {
                    next.weight = cur.weight + g[next.row][next.col];

                    int cur_price = price[next.row][next.col][next.dir_row][next.dir_col][next.dir_steps];
                    if (cur_price != 0) {
                        if (next.weight > cur_price) {
                            continue;   /* not an optimal way */
                        }
                    }

                    price[next.row][next.col][next.dir_row][next.dir_col][next.dir_steps] = next.weight;
                    to_process.insert(next);
                }
            }

            std::vector<std::pair<int, int>> neis = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };
            if (
                (!elves_something_changed) ||       /* part 1 */
                (elves_something_changed && ((cur.dir_steps >= 4) || (cur.dir_row == 0 && cur.dir_col == 0)))   /* part 2 */
                )
            {
                for (auto& nei : neis) {
                    if (!(nei.first == cur.dir_row && nei.second == cur.dir_col) && !(nei.first == -cur.dir_row && nei.second == -cur.dir_col)) {
                        state next;

                        next.row = cur.row + nei.first;
                        next.col = cur.col + nei.second;
                        next.dir_row = nei.first;
                        next.dir_col = nei.second;
                        next.dir_steps = 1;

                        if (next.row >= 0 && next.row < g.size() && next.col >= 0 && next.col < g[0].size()) {
                            next.weight = cur.weight + g[next.row][next.col];

                            int cur_price = price[next.row][next.col][next.dir_row][next.dir_col][next.dir_steps];
                            if (cur_price != 0) {
                                if (next.weight > cur_price) {
                                    continue;
                                }
                            }

                            price[next.row][next.col][next.dir_row][next.dir_col][next.dir_steps] = next.weight;
                            to_process.insert(next);
                        }
                    }
                }
            }
        }

        return -1;
    }
};


int main() {
    auto input = read_input();

    std::cout << "The least heat loss: " << path_finder().analyser(input, false) << std::endl;
    std::cout << "The least heat loss: " << path_finder().analyser(input, true) << std::endl;

    return 0;
}