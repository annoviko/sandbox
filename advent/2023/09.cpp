#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


std::vector<std::vector<std::int64_t>> read_input() {
    std::vector<std::vector<std::int64_t>> result;

    std::fstream stream("input.txt");
    for (std::string line; std::getline(stream, line);) {
        std::stringstream ss(line);

        std::vector<std::int64_t> values;
        while (ss.good()) {
            std::int64_t v;
            ss >> v;

            values.push_back(v);
        }

        result.push_back(std::move(values));
    }

    return result;
}


class predictor {
private:
    std::vector<std::vector<std::int64_t>> m_stats;

public:
    predictor(const std::vector<std::vector<std::int64_t>>& p_stats) : m_stats(p_stats)
    { }

public:
    std::int64_t predict_back() {
        std::int64_t sum = 0;
        
        for (const auto& r : m_stats) {
            sum += predict_sequence(r);
        }

        return sum;
    }


    int64_t predict_front() {
        std::int64_t sum = 0;

        for (auto& r : m_stats) {
            std::reverse(r.begin(), r.end());
            sum += predict_sequence(r);
        }

        return sum;
    }

private:
    std::int64_t predict_sequence(const std::vector<std::int64_t>& p_seq) {
        std::vector<std::vector<std::int64_t>> vals;
        vals.push_back(p_seq);

        std::int64_t row_sum = 1;
        while (row_sum != 0) {
            std::vector<std::int64_t> row;
            row_sum = 0;

            const auto& prev = vals.back();
            for (int i = 1; i < prev.size(); i++) {
                std::int64_t delta = prev[i] - prev[i - 1];

                row.push_back(delta);
                row_sum += delta;
            }

            vals.push_back(row);
        }

        vals.back().push_back(0);
        for (int i = vals.size() - 2; i >= 0; i--) {
            std::int64_t pred = vals[i + 1].back() + vals[i].back();
            vals[i].push_back(pred);
        }

        return vals.front().back();
    }
};


int main() {
    auto input = read_input();

    std::cout << "The sum of back extrapolated values: " << predictor(input).predict_back() << std::endl;
    std::cout << "The sum of front extrapolated values: " << predictor(input).predict_front() << std::endl;

    return 0;
}