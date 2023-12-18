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
    std::int64_t area_by_geometry(const instruction_vec& instr_vec) {   /* O(N), where N - instructions */
        std::vector<std::pair<std::int64_t, std::int64_t>> points = { { 0, 0 } };

        std::int64_t row = 0;
        std::int64_t col = 0;

        std::int64_t boundary_points = 0;

        for (auto& instr : instr_vec) {
            auto step = get_step(instr.dir);

            row += step.first * instr.len;
            col += step.second * instr.len;
            boundary_points += instr.len;

            points.push_back({ row, col });
        }

        /* Shoelace formula */
        std::int64_t area = 0;
        for (int i = 0; i < points.size(); i++) {
            std::int64_t x = points[i].first;
            std::int64_t y_next = (i + 1 < points.size()) ? points[i + 1].second : points.front().second;
            std::int64_t y_prev = (i - 1 >= 0) ? points[i - 1].second : points.back().second;

            area += x * (y_next - y_prev);
        }

        area = std::abs(area / 2);  /* internal area */
        
        /* Pick's theorem */
        std::int64_t i = area - boundary_points / 2 + 1;    /* interior points */

        std::int64_t total_area = i + boundary_points;  /* area = interior points + boundary points */
        return total_area;
    }


    std::int64_t area_by_bfs(const instruction_vec& instr_vec) {    /* Performance: O(M^2), Memory: O(M), where M - actual values in space */
        /* Can be used for the part 1, no way for part 2 (too slow) */

        /* build borders */
        map m;

        std::int64_t row = 0;
        std::int64_t col = 0;
        m[row][col] = true;

        for (const auto& instr : instr_vec) {
            std::pair<std::int64_t, std::int64_t> step = get_step(instr.dir);
            for (int i = 0; i < instr.len; i++) {
                row += step.first;
                col += step.second;

                m[row][col] = true;
            }
        }

        /* find borders */
        std::int64_t lborder = std::numeric_limits<std::int64_t>::max();
        std::int64_t rborder = std::numeric_limits<std::int64_t>::min();

        std::int64_t uborder = std::numeric_limits<std::int64_t>::max();
        std::int64_t dborder = std::numeric_limits<std::int64_t>::min();

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
    std::cout << "Area to keep lava: " << solution().area_by_geometry(read_input(false)) << std::endl;
    std::cout << "Area to keep lava (elves realized their mistake): " << solution().area_by_geometry(read_input(true)) << std::endl;

    return 0;
}