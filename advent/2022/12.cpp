#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <string>


struct position_t {
    int r = -1;
    int c = -1;

    bool is_valid() const { return r >= 0 && c >= 0; }
};


struct field_t {
    std::vector<std::string> heatmap;
    position_t begin;
    position_t target;
};


field_t read_input() {
    std::ifstream stream("input.txt");

    field_t input;
    for (std::string line; std::getline(stream, line);) {
        input.heatmap.push_back(line);

        std::size_t begin = line.find('S');
        if (begin != std::string::npos) {
            input.begin = { (int)input.heatmap.size() - 1, (int)begin };
            input.heatmap.back()[begin] = 'a';
        }

        std::size_t target = line.find('E');
        if (target != std::string::npos) {
            input.target = { (int)input.heatmap.size() - 1, (int)target };
            input.heatmap.back()[target] = 'z';
        }
    }

    return input;
}


class solution {
private:
    field_t m_field;

public:
    solution(const field_t& p_field) : m_field(p_field) {}

public:
    int minimize_steps_to_catch_signal() {
        std::vector<std::vector<int>> distances(m_field.heatmap.size(), std::vector<int>(m_field.heatmap[0].size(), INT_MAX));
        distances[m_field.begin.r][m_field.begin.c] = 0;

        std::queue<position_t> q;
        q.push(m_field.begin);

        return minimize_steps_to_catch_signal(q, distances);
    }

    int minimize_steps_for_hiking() {
        std::queue<position_t> q;
        std::vector<std::vector<int>> distances(m_field.heatmap.size(), std::vector<int>(m_field.heatmap[0].size(), INT_MAX));
        
        for (int r = 0; r < m_field.heatmap.size(); r++) {
            for (int c = 0; c < m_field.heatmap[0].size(); c++) {
                if (m_field.heatmap[r][c] == 'a') {
                    q.push({ r, c });
                    distances[r][c] = 0;
                }
            }
        }

        return minimize_steps_to_catch_signal(q, distances);
    }

private:
    int minimize_steps_to_catch_signal(std::queue<position_t>& q, std::vector<std::vector<int>>& distances) {


        while (!q.empty()) {
            auto cur = q.front();
            q.pop();

            std::vector<position_t> neis = {
                { cur.r, cur.c - 1 },
                { cur.r, cur.c + 1 },
                { cur.r - 1, cur.c },
                { cur.r + 1, cur.c }
            };

            const int cost = distances[cur.r][cur.c] + 1;

            for (const auto& nei : neis) {
                if (nei.r < 0 || nei.c < 0 || nei.r >= m_field.heatmap.size() || nei.c >= m_field.heatmap[0].size()) {
                    continue;
                }

                const int hight_difference = m_field.heatmap[nei.r][nei.c] - m_field.heatmap[cur.r][cur.c];
                if (hight_difference > 1) {
                    continue;
                }

                if (cost >= distances[nei.r][nei.c]) {
                    continue;
                }

                distances[nei.r][nei.c] = cost;
                q.push({ nei.r, nei.c });

                if (nei.r == m_field.target.r && nei.c == m_field.target.c) {
                    return cost;
                }
            }
        }

        return -1;
    }
};


int main() {
    auto input = read_input();
    
    std::cout << "The minimum number of steps to catch the signal: " << solution(input).minimize_steps_to_catch_signal() << std::endl;
    std::cout << "The minimum number of steps for hiking: " << solution(input).minimize_steps_for_hiking() << std::endl;

    return 0;
}