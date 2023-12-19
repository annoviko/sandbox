#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <unordered_map>


struct condition {
    char var;
    char operation;
    std::int64_t value;
    std::string label_to_jump;
};


struct workflow {
    std::vector<condition> flow;
    std::string final;
};


struct input {
    std::unordered_map<std::string, workflow> workflows;
    std::vector<std::unordered_map<char, std::int64_t>> var_entries;
};


class solution {
public:
    std::int64_t accepted_score(input& in) {
        std::int64_t result = 0;

        for (auto& vars : in.var_entries) {
            if (is_accepted(vars, in.workflows)) {
                for (auto& v : vars) {
                    result += v.second;
                }
            }
        }

        return result;
    }

    std::int64_t accepted_xmas_combinations(input& in, std::unordered_map<char, std::pair<std::int64_t, std::int64_t>> ranges, const std::string& key) {
        if (key == "R") {
            return 0;
        }
        else if (key == "A") {
            std::int64_t result = 1;
            for (const auto& r : ranges) {
                std::int64_t lborder = r.second.first;
                std::int64_t rborder = r.second.second;
                result *= (rborder - lborder + 1);
            }
            return result;
        }

        std::int64_t result = 0;
        bool jump_to_final_label = true;

        for (auto& c : in.workflows[key].flow) {
            std::int64_t lborder = ranges[c.var].first;
            std::int64_t rborder = ranges[c.var].second;
            
            std::pair<std::int64_t, std::int64_t> true_range, false_range;
            if (c.operation == '<') {
                true_range = { lborder, c.value - 1 };
                false_range = { c.value, rborder };
            }
            else {
                true_range = { c.value + 1, rborder };
                false_range = { lborder, c.value };
            }

            if (true_range.first <= true_range.second) {
                std::unordered_map<char, std::pair<std::int64_t, std::int64_t>> new_ranges = ranges;
                new_ranges[c.var] = true_range;

                result += accepted_xmas_combinations(in, new_ranges, c.label_to_jump);
            }

            if (false_range.first <= false_range.second) {
                ranges[c.var] = false_range;
            }
            else {
                jump_to_final_label = false;
                break;
            }
        }

        if (jump_to_final_label) {
            result += accepted_xmas_combinations(in, ranges, in.workflows[key].final);
        }

        return result;
    }

private:
    bool is_accepted(std::unordered_map<char, std::int64_t>& vars, std::unordered_map<std::string, workflow>& workflows) {
        std::string cur_key = "in";

        while (cur_key != "R" && cur_key != "A") {
            auto& w = workflows[cur_key];
            cur_key = run(vars, w);
        }

        return cur_key == "A";
    }


    std::string run(std::unordered_map<char, std::int64_t>& vars, workflow& w) {
        for (auto& c : w.flow) {
            const char var_name = c.var;
            const std::int64_t lvalue = vars[var_name];
            const std::int64_t rvalue = c.value;

            if (c.operation == '>' && lvalue > rvalue) {
                return c.label_to_jump;
            }
            else if (c.operation == '<' && lvalue < rvalue) {
                return c.label_to_jump;
            }
        }

        return w.final;
    }
};


input read_input() {
    std::fstream stream("input.txt");

    input result;

    for (std::string line; std::getline(stream, line);) {
        if (line.empty()) {
            break;
        }

        auto pos = line.find("{");

        std::string key = line.substr(0, pos);
        auto pos_prev = pos + 1;

        workflow w;

        while ((pos = line.find(',', pos + 1)) != std::string::npos) {
            std::string expr = line.substr(pos_prev, pos - pos_prev);

            char var = expr[0];
            char oper = expr[1];

            auto pos_dot = expr.find(':');
            std::string str_val = expr.substr(2, pos_dot - 2);
            std::int64_t val = std::stoll(str_val);

            std::string label = expr.substr(pos_dot + 1);
            w.flow.push_back({ var, oper, val, label });

            pos_prev = pos + 1;
        }

        w.final = line.substr(pos_prev);
        w.final.pop_back();
        result.workflows.insert({ key, w });
    }

    for (std::string line; std::getline(stream, line);) {
        std::regex pattern("\\{(.)=(\\d+),(.)=(\\d+),(.)=(\\d+),(.)=(\\d+)\\}");
        std::smatch matches;

        std::regex_search(line, matches, pattern);

        std::unordered_map<char, std::int64_t> vars;

        for (int i = 0; i < 4; i++) {
            const char name = matches[i * 2 + 1].str()[0];
            const std::int64_t val = std::stoll(matches[(i + 1) * 2].str());

            vars[name] = val;
        }

        result.var_entries.push_back(vars);
    }

    return result;
}


int main() {
    auto in = read_input();

    std::cout << "Accepted rating numbers after sorting: " << solution().accepted_score(in) << std::endl;


    std::unordered_map<char, std::pair<std::int64_t, std::int64_t>> xmas_ranges = {
        { 'x', { 1, 4000 } },
        { 'm', { 1, 4000 } },
        { 'a', { 1, 4000 } },
        { 's', { 1, 4000 } }
    };

    std::cout << "Accepted distinct combinations of ratings: " << solution().accepted_xmas_combinations(in, xmas_ranges, "in") << std::endl;

    return 0;
}