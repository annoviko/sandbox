#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <unordered_map>


using graph_t = std::unordered_map<std::string, std::vector<std::string>>;


class solution {
private:
    graph_t m_g;
    std::unordered_map<std::string, std::uint64_t> cache_part_1;
    std::unordered_map<std::string, std::unordered_map<char, std::uint64_t>> cache_part_2;

public:
    solution(const graph_t& g) : m_g(g) { }

public:
    std::uint64_t count_paths() {
        cache_part_1.clear();
        return dfs("you");
    }

    std::uint64_t count_paths_via_fft_dac() {
        cache_part_2.clear();
        return dfs_fft_dac("svr", 0);
    }

private:
    std::uint64_t dfs(const std::string& cur) {
        if (cur == "out") {
            return 1;
        }

        auto iter = cache_part_1.find(cur);
        if (iter != cache_part_1.cend()) {
            return iter->second;
        }

        std::uint64_t total = 0;
        for (const auto& nei : m_g[cur]) {
            const auto path = dfs(nei);
            total += path;
        }

        cache_part_1[cur] = total;
        return total;
    }


    std::uint64_t dfs_fft_dac(const std::string& cur, char condition) {
        if (cur == "out") {
            return (condition == 0x03) ? 1 : 0;
        }
        else if (cur == "fft") {
            condition |= 0x02;
        }
        else if (cur == "dac") {
            condition |= 0x01;
        }

        auto iter_node = cache_part_2.find(cur);
        if (iter_node != cache_part_2.cend()) {
            auto iter = iter_node->second.find(condition);
            if (iter != iter_node->second.cend()) {
                return iter->second;
            }
        }

        std::uint64_t total = 0;
        for (const auto& nei : m_g[cur]) {
            const auto path = dfs_fft_dac(nei, condition);
            total += path;
        }

        cache_part_2[cur][condition] = total;
        return total;
    }
};


graph_t read_input() {
    std::fstream stream("input.txt");

    graph_t g;
    for (std::string line; std::getline(stream, line); ) {
        std::stringstream ss(line);

        std::string from;
        ss >> from;
        from.pop_back();

        g[from] = { };
        for (std::string to; ss >> to;) {
            g[from].push_back(to);
        }
    }

    return g;
}


int main() {
    const auto input = read_input();

    std::cout << "The number paths from 'you' to 'out': " << solution(input).count_paths() << std::endl;
    std::cout << "The number paths from 'svr' to 'out' via 'dac' and 'fft': " << solution(input).count_paths_via_fft_dac() << std::endl;

    return 0;
}