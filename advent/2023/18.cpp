#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <unordered_map>

                             /* [row]                [col] -> bool */
using map = std::unordered_map<std::int64_t, std::unordered_map<std::int64_t, bool>>;
using color_map = std::unordered_map<std::int64_t, std::unordered_map<std::int64_t, std::int64_t>>;


struct instruction {
    char dir;
    std::int64_t  len;
    std::string color;
};


using instruction_vec = std::vector<instruction>;


class solution {
public:
    std::int64_t count_bfs_area(const instruction_vec& instr_vec) {
        /* build borders */
        map m;

        std::int64_t row = 0;
        std::int64_t col = 0;
        m[row][col] = true;

        for (const auto& instr : instr_vec) {
            std::pair<int, int> step = get_step(instr.dir);
            for (int i = 0; i < instr.len; i++) {
                row += step.first;
                col += step.second;

                m[row][col] = true;
            }
        }

        /* find borders */
        std::int64_t lborder = std::numeric_limits<int>::max();
        std::int64_t rborder = std::numeric_limits<int>::min();

        std::int64_t uborder = std::numeric_limits<int>::max();
        std::int64_t dborder = std::numeric_limits<int>::min();

        for (auto& r : m) {
            uborder = std::min(r.first, uborder);
            dborder = std::max(r.first, dborder);

            for (auto& c : r.second) {
                lborder = std::min(c.first, lborder);
                rborder = std::max(c.first, rborder);
            }
        }

        /* expand borders */
        const std::int64_t border_expander = 2;
        rborder += border_expander;
        lborder -= border_expander;
        uborder -= border_expander;
        dborder += border_expander;

        /* call bfs with to fill outside area */
        map outside_visited;

        std::queue<std::pair<std::int64_t, std::int64_t>> to_process;
        to_process.push({ uborder, lborder });

        outside_visited[uborder][lborder] = true;

        while (!to_process.empty()) {
            auto cur = to_process.front();
            to_process.pop();

            auto neis = get_neighbors(cur.first, cur.second, lborder, rborder, uborder, dborder);

            for (auto& nei : neis) {
                /* not a perimeter */
                if (m[nei.first][nei.second]) {
                    continue;
                }

                /* not visited */
                if (!outside_visited[nei.first][nei.second]) {
                    outside_visited[nei.first][nei.second] = true;
                    to_process.push(nei);
                }
            }
        }

        /* count */
        std::int64_t result = 0;
        for (std::int64_t r = uborder; r <= dborder; r++) {
            for (std::int64_t c = lborder; c <= rborder; c++) {
                if (!outside_visited[r][c]) {
                    result++;
                }
            }
        }

        return result;
    }

private:
    std::vector<std::pair<std::int64_t, std::int64_t>> get_neighbors(const std::int64_t row, const std::int64_t col, const std::int64_t lborder, const std::int64_t rborder, const std::int64_t uborder, const std::int64_t dborder) {
        std::vector<std::pair<std::int64_t, std::int64_t>> candidates = { { row - 1, col }, { row + 1, col }, { row, col - 1 }, { row, col + 1 } };
        std::vector<std::pair<std::int64_t, std::int64_t>> neis;

        for (auto& c : candidates) {
            if (c.first >= uborder && c.first <= dborder && c.second >= lborder && c.second <= rborder) {
                neis.push_back(c);
            }
        }

        return neis;
    }

    std::pair<std::int64_t, std::int64_t> get_step(const char dir) {
        switch (dir) {
            case 'R': return { 0, 1 };
            case 'L': return { 0, -1 };
            case 'U': return { -1, 0 };
            case 'D': return { 1, 0 };
        }
    }
};



instruction_vec read_input(bool elves_made_mistake) {
    std::fstream stream("input.txt");

    instruction_vec result;
    for (std::string line; std::getline(stream, line);) {
        instruction instr;

        std::stringstream ss(line);
        if (!elves_made_mistake) {
            ss >> instr.dir >> instr.len >> instr.color;
        }
        else {
            ss >> instr.dir >> instr.len >> instr.color;

            instr.color = instr.color.substr(2, instr.color.size() - 3);
            switch (instr.color.back()) {
            case '0':
                instr.dir = 'R';
                break;

            case '1':
                instr.dir = 'D';
                break;

            case '2':
                instr.dir = 'L';
                break;

            case '3':
                instr.dir = 'U';
                break;

            default:
                throw std::exception("invalid direction");
            }

            std::string str_len = instr.color;
            str_len.pop_back();

            std::int64_t len = std::stoi(str_len, nullptr, 16);
            instr.len = len;
        }

        result.push_back(instr);
    }

    return result;
}


int main() {
    std::cout << "Area to keep lava: " << solution().count_bfs_area(read_input(false)) << std::endl;

    return 0;
}