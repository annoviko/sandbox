#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <stack>


enum direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};


struct position {
    std::int64_t r = -1;
    std::int64_t c = -1;

    bool is_valid() { return c != -1 && r != -1; }

    bool operator==(const position& p) const {
        return p.r == r && p.c == c;
    }

    bool operator!=(const position& p) const {
        return !(*this == p);
    }
};


auto position_hash = [](const position& s) {
    return std::hash<std::int64_t>()(s.r) ^ std::hash<std::int64_t>()(s.c);
};


auto position_equal = [](const position& lhs, const position& rhs) {
    return lhs.r == rhs.r && lhs.c == rhs.c;
};


struct graph_result {
    std::int64_t fathest_distance = 0;
    std::int64_t enclosed_tiles = 0;
};


std::vector<std::string> read_input() {
    std::fstream stream("input.txt");
    std::vector<std::string> result;

    for (std::string line; std::getline(stream, line);) {
        result.push_back(line);
    }

    return result;
}


class solution {
    std::vector<std::string> g;

public:
    solution(const std::vector<std::string>& p_g) : 
        g(p_g)
    { }

public:
    graph_result find_loop_size() {
        for (int r = 0; r < g.size(); r++) {
            for (int c = 0; c < g[r].size(); c++) {
                if (g[r][c] != 'S') {
                    continue;
                }

                return bfs(r, c);
            }
        }

        throw std::runtime_error("incorrect graph");
    }

private:
    graph_result bfs(const std::int64_t r, const std::int64_t c) {
        std::queue<position> to_process;
        to_process.push({ r, c });

        std::vector<std::vector<std::int64_t>> len(g.size(), std::vector<std::int64_t>(g[0].size(), -1));
        len[r][c] = 0;

        std::int64_t max_len = -1;
        position meeting_point;

        while (!to_process.empty()) {
            auto p = to_process.front();
            to_process.pop();

            std::int64_t cur_len = len[p.r][p.c];

            auto neis = get_neighbors(p.r, p.c);
            bool way_to_nowhere = (neis.size() == 1);
            for (auto& nei : neis) {
                char c_nei = g[nei.r][nei.c];
                char c_cur = g[p.r][p.c];

                if (!is_bidirectional_neighbor(nei, p)) {
                    continue;
                }

                if (len[nei.r][nei.c] == -1) {
                    len[nei.r][nei.c] = cur_len + 1;
                    to_process.push(nei);
                }
                else {
                    std::int64_t len_to_nei = len[nei.r][nei.c];
                    if (max_len < len_to_nei) {
                        max_len = len_to_nei;
                        meeting_point = p;
                    }
                    
                    if (max_len < cur_len) {
                        max_len = cur_len;
                        meeting_point = p;
                    }
                }
            }
        }


        graph_result result;
        result.fathest_distance = max_len;
        result.enclosed_tiles = 0;

        /* restore path by running bfs from meeting point */
        std::vector<std::vector<bool>> visited(g.size(), std::vector<bool>(g[0].size(), false));

        to_process.push(meeting_point);
        visited[meeting_point.r][meeting_point.c] = true;

        position p1, p2;
        position alternative;   /* if we won't have anything in right or left */
        bool is_alternative = false;

        auto neis = get_neighbors(meeting_point.r, meeting_point.c);
        for (auto& nei : neis) {
            visited[nei.r][nei.c] = true;
            alternative = nei;
            break;
        }

        std::vector<std::string> colors(g.size(), std::string(g[0].size(), ' '));

        while (!to_process.empty()) {
            auto p = to_process.front();
            to_process.pop();

            colors[p.r][p.c] = '*';
            if (g[p.r][p.c] == 'S') {
                if (to_process.empty() && !is_alternative) {
                    is_alternative = true;
                    visited[alternative.r][alternative.c] = false;
                    visited[r][c] = false;
                    to_process.push(meeting_point); /* time to consider alternative */
                }

                continue;
            }

            auto neis = get_neighbors(p.r, p.c);
            for (auto& nei : neis) {
                if (!is_bidirectional_neighbor(p, nei)) {
                    continue;
                }

                if (visited[nei.r][nei.c]) {
                    continue;
                }

                if (g[nei.r][nei.c] == 'S') {
                    if (!is_alternative) {
                        p1 = p;
                    }
                    else {
                        p2 = p;
                    }
                }

                to_process.push(nei);
                visited[nei.r][nei.c] = true;
            }

            if (to_process.empty() && !is_alternative) {
                is_alternative = true;
                visited[alternative.r][alternative.c] = false;
                visited[r][c] = false;
                to_process.push(meeting_point); /* time to consider alternative */
            }
        }

        /* identify S */
        if (p1.r > p2.r) {
            std::swap(p1, p2);
        }

        if (p1.r == p2.r) {
            g[r][c] = '-';
        }
        else if (p1.c == p2.c) {
            g[r][c] = '|';
        }
        else if (p1.r < p2.r && p1.c < p2.c) {
            if (p1.r == r) {
                g[r][c] = '7';
            }
            else {
                g[r][c] = 'L';
            }
        }
        else if (p1.r < p2.r && p1.c > p2.c) {
            if (p1.r == r) {
                g[r][c] = 'F';
            }
            else {
                g[r][c] = 'J';
            }
        }
        
        if (g[r][c] == 'S') {
            throw std::runtime_error("S is not identified");
        }

        /* bfs to fill with colors */
        visited = std::vector<std::vector<bool>>(g.size(), std::vector<bool>(g[0].size(), false));

        to_process.push({ r, c });
        visited[r][c] = true;

        neis = get_neighbors(r, c);
        colors[r][c] = '*';

        for (auto& nei : neis) {
            visited[nei.r][nei.c] = true;
            alternative = nei;
            break;
        }

        char index_color_external = -1;
        bool is_block_removed = false;

        while (!to_process.empty()) {
            auto p = to_process.front();
            to_process.pop();

            if (p != position{ r, c } && !is_block_removed) {
                is_block_removed = true;
                visited[alternative.r][alternative.c] = false;
            }

            auto neis = get_neighbors(p.r, p.c);    /* get neighbors as pipes */
            for (auto& nei : neis) {
                if (colors[nei.r][nei.c] != '*') {
                    continue;
                }

                if (visited[nei.r][nei.c]) {
                    continue;
                }

                to_process.push(nei);

                visited[nei.r][nei.c] = true;
                direction dir = get_direction(p, nei);

                std::vector<position> left_candidates;
                std::vector<position> right_candidates;

                switch (dir) {
                case RIGHT:
                    left_candidates = { { nei.r - 1, nei.c }, { p.r - 1, p.c } };
                    right_candidates = { { nei.r + 1, nei.c }, { p.r + 1, p.c } };
                    break;

                case LEFT:
                    left_candidates = { { nei.r + 1, nei.c }, { p.r + 1, p.c } };
                    right_candidates = { { nei.r - 1, nei.c }, { p.r - 1, p.c } };
                    break;

                case UP:
                    left_candidates = { { nei.r, nei.c - 1 }, { p.r, p.c - 1 } };
                    right_candidates = { { nei.r, nei.c + 1 }, { p.r, p.c + 1 } };
                    break;

                case DOWN:
                    left_candidates = { { nei.r, nei.c + 1 }, { p.r, p.c + 1 } };
                    right_candidates = { { nei.r, nei.c - 1 }, { p.r, p.c - 1 } };
                    break;
                }

                for (auto& left_candidate : left_candidates) {
                    if (is_inside(left_candidate)) {
                        if (bfs_fill('O', left_candidate, colors)) {
                            index_color_external = 'O';
                        }
                    }
                }

                for (auto& right_candidate : right_candidates) {
                    if (is_inside(right_candidate)) {
                        if (bfs_fill('I', right_candidate, colors)) {
                            index_color_external = 'I';
                        }
                    }
                }
            }
        }

        for (auto& row : colors) {
            for (auto v : row) {
                if (v == '*' || v == ' ') {
                    continue;
                }

                if (v != index_color_external) {
                    result.enclosed_tiles++;
                }
            }
        }

#if 0   /* visualization */
#if 1
        std::ofstream stream("out.txt");
#else
#define stream std::cout
#endif
        for (int i = 0; i < g.size(); i++) {
            for (int j = 0; j < g[i].size(); j++) {
                if (colors[i][j] == '*') {
#if 1
                    stream << g[i][j];
#else
                    stream << colors[i][j];
#endif
                }
                else {
                    stream << colors[i][j];
                }
            }

            stream << std::endl;
        }
#endif

        return result;
    }


    std::vector<position> get_neighbors_from_path(const position& pos, const std::vector<std::string>& path) {
        std::vector<position> candidates = {
            { pos.r + 1, pos.c }, { pos.r - 1, pos.c }, { pos.r, pos.c + 1 }, { pos.r, pos.c - 1 }
        };

        std::vector<position> neis;
        for (auto p : candidates) {
            if (p.r >= 0 && p.r < g.size() && p.c >= 0 && p.c < g[p.r].size() && path[p.r][p.c] == '*') {
                neis.push_back(p);
            }
        }

        return neis;
    }


    std::vector<position> get_neighbors_from_colors(const position& pos, const std::vector<std::string>& colors) {
        std::vector<position> candidates = {
            { pos.r + 1, pos.c }, { pos.r - 1, pos.c }, { pos.r, pos.c + 1 }, { pos.r, pos.c - 1 }
        };

        std::vector<position> neis;
        for (auto p : candidates) {
            if (p.r >= 0 && p.r < g.size() && p.c >= 0 && p.c < g[p.r].size() && colors[p.r][p.c] != '*') {
                neis.push_back(p);
            }
        }

        return neis;
    }


    bool bfs_fill(const char color, const position& p, std::vector<std::string>& colors) {
        if (colors[p.r][p.c] == '*') {
            return false;
        }

        if (colors[p.r][p.c] != ' ') {
            return false;
        }

        bool is_touch_border = false;

        std::queue<position> to_process;
        to_process.push(p);
        colors[p.r][p.c] = color;

        while (!to_process.empty()) {
            auto cur = to_process.front();
            to_process.pop();

            is_touch_border |= is_border(cur);

            auto neis = get_neighbors_from_colors(cur, colors);
            for (auto& nei : neis) {
                if (colors[nei.r][nei.c] == '*') {
                    continue;
                }

                if (colors[nei.r][nei.c] != ' ') {
                    if (colors[nei.r][nei.c] != color) {
                        throw std::runtime_error("incorrect coloring");
                    }
                    continue;
                }

                colors[nei.r][nei.c] = color;
                to_process.push(nei);
            }
        }

        return is_touch_border;
    }


    direction get_direction(position& from, position& to) {
        if (to.r == from.r) {
            if (to.c > from.c) {
                return RIGHT;
            }
            else if (to.c < from.c) {
                return LEFT;
            }
        }
        else if (to.c == from.c) {
            if (to.r > from.r) {
                return DOWN;
            }
            else if (to.r < from.r) {
                return UP;
            }
        }

        throw std::runtime_error("wrong movement");
    }


    bool is_border(const position& p) {
        if ((p.r == g.size() - 1) || (p.r == 0) || (p.c == g[0].size() - 1) || (p.c == 0)) {
            return true;
        }

        return false;
    }


    bool is_inside(const position& p) {
        if (p.r >= 0 && p.r < g.size() && p.c >= 0 && p.c < g[p.r].size()) {
            return true;
        }

        return false;
    }


    std::vector<position> get_neighbors(const std::int64_t r, const std::int64_t c) {
        char cur_pipe_sgm = g[r][c];
        std::vector<position> candidates = { };

        switch (cur_pipe_sgm) {
        case '|':
            candidates = { { r - 1, c }, { r + 1, c } };
            break;

        case '-':
            candidates = { { r, c - 1 }, { r, c + 1 } };
            break;

        case 'L':
            candidates = { { r - 1, c }, { r, c + 1 } };
            break;

        case 'J':
            candidates = { { r - 1, c }, { r, c - 1 } };
            break;

        case '7':
            candidates = { { r, c - 1 }, { r + 1, c } };
            break;

        case 'F':
            candidates = { { r, c + 1 }, { r + 1, c } };
            break;

        case 'S':
            candidates = { { r, c + 1 }, { r, c - 1 }, { r + 1, c }, { r - 1, c } };
            break;
        }

        std::vector<position> neighbors;
        for (auto& p : candidates) {
            if (p.r < g.size() && p.r >= 0 && p.c < g[p.r].size() && p.c >= 0 && g[p.r][p.c] != '.') {
                neighbors.push_back(p);
            }
        }

        return neighbors;
    }

    bool is_bidirectional_neighbor(const position& nei, const position& cur) {
        if (g[nei.r][nei.c] == 'S') {
            return true;
        }

        auto neis = get_neighbors(nei.r, nei.c);
        if (std::find(neis.begin(), neis.end(), cur) != neis.end()) {
            return true;
        }

        return false;
    }
};


int main() {
    auto input = read_input();
    auto result = solution(input).find_loop_size();

    std::cout << "Steps from the starting position to the point farthest from the starting position: " << result.fathest_distance << std::endl;
    std::cout << "Enclosed tiles by the loop: " << result.enclosed_tiles << std::endl;

    return 0;
}