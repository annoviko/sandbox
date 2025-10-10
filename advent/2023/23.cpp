#include <fstream>
#include <queue>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <iostream>


using field_t = std::vector<std::string>;


field_t read_input() {
    std::fstream file("input.txt");

    field_t result;
    for (std::string line; std::getline(file, line);) {
        result.push_back(line);
    }

    return result;
}


struct position_t {
    int r;
    int c;

public:
    bool operator<(const position_t& p_other) const {
        if (r < p_other.r) {
            return true;
        }
        else if (r == p_other.r) {
            return c < p_other.c;
        }
        else {
            return false;
        }
    }

    bool operator==(const position_t& p_other) const {
        return r == p_other.r && c == p_other.c;
    }
};


struct nei_t {
    position_t p;
    int w;
};

using graph_t = std::map<position_t, std::vector<nei_t>>;

class solution {
private:
    field_t m_field;
    std::vector<std::vector<bool>> m_visited;
    graph_t m_g;
    bool m_is_slippery = true;

public:
    solution(const field_t& field, bool is_slippery) :
        m_field(field),
        m_is_slippery(is_slippery),
        m_visited(m_field.size(), std::vector<bool>(m_field[0].size(), false))
    {
        build_graph();
    }

public:
    std::uint64_t find_longest_trail() {
        position_t begin{ 0, m_field[0].find('.') };
        return dfs({ begin, 0 });
    }

private:
    std::uint64_t dfs(const nei_t& n) {
        if (n.p.r == m_field.size() - 1) {
            return n.w;
        }

        m_visited[n.p.r][n.p.c] = true;

        std::uint64_t distance = 0;

        for (const nei_t& nei : m_g[n.p]) {
            if (m_visited[nei.p.r][nei.p.c]) {
                continue;
            }

            std::uint64_t nei_dist = dfs(nei);
            if (nei_dist) {
                distance = std::max(distance, nei_dist + n.w);
            }
        }

        m_visited[n.p.r][n.p.c] = false;
        return distance;
    }

    void build_graph() {
        position_t begin{ 0, m_field[0].find('.') };

        std::set<position_t> visited;
        visited.insert(begin);

        std::queue<position_t> to_proc;
        to_proc.push(begin);

        while (!to_proc.empty()) {
            position_t cur = to_proc.front();
            to_proc.pop();

            std::map<position_t, int> neis = find_neighbors(cur.r, cur.c);

            for (const auto& n : neis) {
                m_g[{ cur.r, cur.c }].push_back({ n.first, n.second });
                if (visited.count(n.first) > 0) {
                    continue;
                }

                visited.insert(n.first);
                to_proc.push(n.first);
            }
        }
    }

    void print_graph() {
        std::cout << std::endl;
        for (const auto& pair : m_g) {
            std::cout << '(' << pair.first.r << ", " << pair.first.c << "): [";
            for (const auto& nei : pair.second) {
                std::cout << " (" << nei.p.r << ", " << nei.p.c << " - " << nei.w << ')';
            }
            std::cout << " ]" << std::endl;
        }
    }

    std::vector<position_t> get_neighbors(const position_t& cur) {
        const std::vector<position_t> dirs = {
            { cur.r + 1, cur.c },
            { cur.r - 1, cur.c },
            { cur.r, cur.c + 1 },
            { cur.r, cur.c - 1 },
        };

        std::vector<position_t> neis;
        for (const auto& d : dirs) {
            if (d.r < 0 || d.r >= m_field.size() || d.c < 0 || d.c >= m_field[0].size()) {
                continue;
            }

            if (m_field[d.r][d.c] == '#') {
                continue;
            }

            if (m_is_slippery && !is_slope_neighbor(cur, d)) {
                continue;
            }

            neis.push_back(d);
        }

        return neis;
    }

    bool is_slope_neighbor(const position_t slope, const position_t neibour) {
        bool is_acceptable = true;
        if (m_is_slippery) {
            switch (m_field[slope.r][slope.c]) {
            case '>':
                is_acceptable = (neibour.c > slope.c);
                break;
            case '<':
                is_acceptable = (neibour.c < slope.c);
                break;
            case '^':
                is_acceptable = (neibour.r < slope.r);
                break;
            case 'v':
                is_acceptable = (neibour.r > slope.r);
                break;
            case '#':
                is_acceptable = false;
                break;
            case '.':
                is_acceptable = true;
                break;
            }
        }

        return is_acceptable;
    }

    bool is_crossroad_cell(const position_t& p) {
        return (p.r == 0) || (p.r == m_field.size() - 1) || get_neighbors(p).size() > 2;
    }

    std::map<position_t, int> find_neighbors(int r, int c) {
        std::queue<position_t> to_proc;
        to_proc.push({ r, c });

        std::vector<std::vector<bool>> visited{ m_field.size(), std::vector<bool>(m_field[0].size(), false) };
        visited[r][c] = true;

        std::map<position_t, int> neis;
        std::queue<position_t> next;

        int distance = 0;

        while (!to_proc.empty() || !next.empty()) {
            if (to_proc.empty()) {
                distance++;
                to_proc = std::move(next);
            }

            position_t cur = to_proc.front();
            to_proc.pop();

            if (!(r == cur.r && c == cur.c) && is_crossroad_cell(cur)) {
                auto iter = neis.find(cur);
                if (iter != neis.cend()) {
                    iter->second = std::max(iter->second, distance);
                }
                else {
                    neis[cur] = distance;
                }
                continue;
            }

            const auto dirs = get_neighbors(cur);

            for (const auto& d : dirs) {
                if (visited[d.r][d.c]) {
                    if (d.r == r && d.c == c) {
                        continue;
                    }

                    bool is_crossroad = is_crossroad_cell(d);
                    if (!is_crossroad) {
                        continue;
                    }
                }

                visited[d.r][d.c] = true;

                if (m_is_slippery && !is_slope_neighbor(cur, d)) {
                    continue;
                }

                next.push(d);
            }
        }

        return neis;
    }
};


int main() {
    field_t field = read_input();

    std::cout << "The longest path with slippery slopes: " << solution(field, true).find_longest_trail() << std::endl;
    std::cout << "The longest path: " << solution(field, false).find_longest_trail() << std::endl;

    return 0;
}
