#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>


std::vector<std::vector<int>> read_input() {
    std::ifstream stream("input.txt");

    std::vector<std::vector<int>> input;

    std::string line;
    while (std::getline(stream, line)) {
        if (line.empty()) {
            continue;
        }

        std::stringstream ss(line);
        std::vector<int> row;

        int value = 0;
        while (ss >> value) {
            row.push_back(value);
        }

        input.push_back(row);
    }

    return input;
}


int main() {
    auto in = read_input();

    int cur_row = 0;

    std::vector<int> dist = { in[0][0] };

    for (int i = 0; i < in.size() - 1; i++) {
        std::vector<int> next_dist(dist.size() + 1, 0);

        cur_row++;
        for (int cur_col = 0; cur_col < dist.size(); cur_col++) {
            for (int nei_col = cur_col; nei_col < cur_col + 2; nei_col++) {
                int candidate = dist[cur_col] + in[cur_row][nei_col];
                if (candidate > next_dist[nei_col]) {
                    next_dist[nei_col] = candidate;
                }
            }
        }

        dist = std::move(next_dist);
    }

    int max_dist = 0;
    for (int d : dist) {
        max_dist = std::max(max_dist, d);
    }

    std::cout << "Maximum Path Sum: " << max_dist << std::endl;
    return 0;
}
