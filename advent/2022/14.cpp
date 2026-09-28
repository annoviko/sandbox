#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>


struct position_t {
    int r = 0;
    int c = 0;
};


struct reservoir_t {
    position_t source;
    std::vector<std::string> field;
};


const int SOURCE_COLUMN = 500;


reservoir_t read_input(bool infinite_floor = false) {
    int rmax = 0, rmin = 0, cmax = 0, cmin = INT_MAX;

    std::vector<std::vector<position_t>> seq;
    reservoir_t descr;

    std::fstream stream("input.txt");
    for (std::string line; std::getline(stream, line);) {
        std::vector<position_t> rock_formation;

        for (std::size_t pos = 0; pos != std::string::npos && pos < line.size();) {
            int row, col;

            std::size_t pos_end = line.find(',', pos);
            col = std::stoi(line.substr(pos, pos_end - pos));
            pos = pos_end + 1;  /* x + comma */

            pos_end = line.find(' ', pos);
            row = std::stoi(line.substr(pos, pos_end - pos));
            
            pos = pos_end;
            pos_end = line.find(' ', pos);
            if (pos_end != std::string::npos) {
                pos += 4; /* space -> space */
            }

            rock_formation.push_back({ row, col });

            rmax = std::max(row, rmax);
            cmax = std::max(col, cmax);
            cmin = std::min(col, cmin);
        }

        seq.push_back(rock_formation);
    }

    int height = rmax - rmin + 1;
    int width = cmax - cmin + 1;

    if (infinite_floor) {
        height += 2;

        for (const auto& s : seq) {
            for (const auto& p : s) {
                const int spread = height - (height - p.r) + 2;

                const int right = p.c + spread;
                const int left = p.c - spread;

                cmin = std::min(cmin, left);
                cmax = std::max(cmax, right);
            }
        }

        width = cmax - cmin;
    }

    int column_offset = (cmin < 0) ? -cmin : 0;

    std::vector<std::string> field(height, std::string(width, '.'));

    if (infinite_floor) {
        field.back() = std::string(width, '#');
    }

    for (const auto& formation : seq) {
        for (int i = 1; i < formation.size(); i++) {
            const position_t& from = formation[i - 1];
            const position_t& to = formation[i];

            int dr = 0, dc = 0;
            if (from.r == to.r) {
                dc = (from.c > to.c) ? -1 : 1;
            }
            else {
                dr = (from.r > to.r) ? -1 : 1;
            }

            int r = from.r;
            int c = from.c;
            while (r != to.r || c != to.c) {
                field[r - rmin][c - cmin + column_offset] = '#';

                r += dr;
                c += dc;
            }

            field[to.r - rmin][to.c - cmin + column_offset] = '#';
        }
    }

    descr.field = std::move(field);
    descr.source = { 0, SOURCE_COLUMN - cmin + column_offset };

    return descr;
}


class solution {
private:
    std::vector<std::string> field;
    position_t source;

public:
    solution(const reservoir_t& r) :
        field(r.field),
        source(r.source)
    {}

public:
    int count_stable_sand() {
        int counter = 0;
        while (simulate()) {
            counter++;

            if (field[source.r][source.c] == 'o') {
                break;
            }
        }

        return counter;
    }

private:
    bool simulate() {
        position_t p = source;
        
        while(true) {
            const position_t cur = p;

            const std::vector<position_t> movements = {
                { p.r + 1, p.c },
                { p.r + 1, p.c - 1 },
                { p.r + 1, p.c + 1 }
            };

            for (const auto& next : movements) {
                if ((next.r >= field.size()) || (next.c < 0) || (next.c >= field[0].size())) {
                    return false;
                }

                if (field[next.r][next.c] == '.') {
                    p = next;
                    break;
                }
            }

            if ((p.r == cur.r) && (p.c == cur.c)) {
                field[p.r][p.c] = 'o';
                return true;    /* is not moving - stable position */
            }
        }
    }
};


int main() {
    auto descr = read_input();

    int stable_units = solution(descr).count_stable_sand();
    std::cout << "Units of sand come to rest: " << stable_units << std::endl;

    descr = read_input(true);
    stable_units = solution(descr).count_stable_sand();
    std::cout << "Units of sand come to rest (infinite floor): " << stable_units << std::endl;

    return 0;
}