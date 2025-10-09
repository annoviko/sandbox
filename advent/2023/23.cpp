#include <fstream>
#include <queue>
#include <vector>
#include <string>
#include <map>
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


class solution {
private:
    field_t m_field;
    std::vector<std::vector<bool>> m_visited;
    std::map<position_t, int> m_cache;
    bool m_is_slippery = true;

public:
    solution(const field_t& field, bool is_slippery) :
        m_field(field),
        m_is_slippery(is_slippery),
        m_visited(m_field.size(), std::vector<bool>(m_field[0].size(), false))
    { }

public:
    std::uint64_t find_longest_trail() {
        position_t begin{ 0, m_field[0].find('.') };
        return dfs(begin) - 1;
    }

private:
    std::uint64_t dfs(const position_t& p) {
        if (p.r == m_field.size() - 1) {
            return 1;
        }

#if 0
        std::uint64_t debug_value = -1;
        auto iter = m_cache.find(p);
        if (iter != m_cache.cend()) {
            debug_value = iter->second;
        }
#endif

        m_visited[p.r][p.c] = true;

        const std::vector<position_t> neis = {
            { p.r + 1, p.c },
            { p.r - 1, p.c },
            { p.r, p.c + 1 },
            { p.r, p.c - 1 },
        };

        std::uint64_t distance = 0;

        for (const position_t nei : neis) {
            if (nei.r >= m_field.size() || nei.r < 0 || nei.c >= m_field[0].size() || nei.c < 0) {
                continue;
            }

            if (m_field[nei.r][nei.c] == '#') {
                continue;
            }

            bool is_acceptable = true;
            if (m_is_slippery) {
                switch (m_field[nei.r][nei.c]) {
                case '>':
                    is_acceptable = (nei == position_t{ p.r, p.c + 1 });
                    break;

                case '<':
                    is_acceptable = (nei == position_t{ p.r, p.c - 1 });
                    break;

                case '^':
                    is_acceptable = (nei == position_t{ p.r - 1, p.c });
                    break;

                case 'v':
                    is_acceptable = (nei == position_t{ p.r + 1, p.c });
                    break;

                default:
                    break;
                }
            }

            if (!is_acceptable) {
                continue;
            }

            if (m_visited[nei.r][nei.c]) {
                continue;
            }

            std::uint64_t nei_dist = dfs(nei);
            if (nei_dist > 0) {
                /* there is a path in this branch */
                distance = std::max(distance, nei_dist + 1);
            }
        }

#if 0
        if (debug_value != -1 && debug_value < distance) {
            std::cout << "Problem detected." << std::endl;
        }
        m_cache[p] = distance;
#endif
        m_visited[p.r][p.c] = false;
        return distance;
    }
};


int main() {
    field_t field = read_input();

    std::cout << solution(field, true).find_longest_trail() << std::endl;
    std::cout << solution(field, false).find_longest_trail() << std::endl;

    return 0;
}
