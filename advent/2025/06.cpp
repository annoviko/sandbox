#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>


struct task_t {
    std::vector<std::uint64_t> values;
    char operation;
};


class solution {
private:
    std::vector<task_t> tasks;

public:
    solution(const std::vector<task_t>& t) : tasks(t) { }

public:
    std::uint64_t eval_grand_total() const {
        std::uint64_t grand_total = 0;
        for (const auto& t : tasks) {
            std::uint64_t r = (t.operation == '+') ? 0 : 1;
            for (const auto v : t.values) {
                switch (t.operation) {
                case '+':
                    r += v;
                    break;

                case '*':
                    r *= v;
                    break;
                }
            }

            grand_total += r;
        }

        return grand_total;
    }

    std::uint64_t eval_cephalopod_grand_total() const {

    }
};


std::vector<task_t> read_input() {
    std::fstream stream("input.txt");
    std::vector<task_t> result;

    for (std::string line; std::getline(stream, line);) {
        if (line[0] == '*' || line[0] == '+') {
            int k = 0;
            for (int i = 0; i < result.size(); i++) {
                for (; line[k] == ' '; k++);
                result[i].operation = line[k];
                k++;
            }
        }
        else {
            std::stringstream ss(line);
            std::uint64_t value = 0;

            for (int i = 0; ss >> value; i++) {
                if (i >= result.size()) {
                    result.push_back({ });
                }

                result[i].values.push_back(value);
            }
        }
    }

    return result;
}


std::vector<task_t> read_cephalopod_input() {
    std::fstream stream("input.txt");
    std::vector<task_t> result;

    std::vector<std::string> content;
    for (std::string line; std::getline(stream, line);) {
        content.push_back(line);
    }

    task_t cur_task;
    std::string cur_value;
    for (int c = 0; c < content[0].size(); c++) {
        cur_value.clear();
        for (int r = 0; r < content.size() - 1; r++) {
            if (content[r][c] == ' ') {
                continue;
            }

            cur_value += content[r][c];
        }

        if (cur_value.empty()) {
            result.push_back(cur_task);
            cur_task.values.clear();
        }
        else {
            cur_task.values.push_back(std::stoull(cur_value));
        }
    }

    result.push_back(cur_task);

    for (int i = 0, k = 0; i < content.back().size(); i++) {
        const char operation = content.back()[i];
        if (operation == ' ') {
            continue;
        }

        result[k].operation = operation;
        k++;
    }

    return result;
}


int main() {
    auto input = read_input();
    std::cout << "The grand total: " << solution(input).eval_grand_total() << std::endl;

    input = read_cephalopod_input();
    std::cout << "The grand total (cephalopod math): " << solution(input).eval_grand_total() << std::endl;

    return 0;
}