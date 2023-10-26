#include <fstream>
#include <iostream>
#include <memory>
#include <regex>
#include <string>
#include <vector>
#include <unordered_map>


struct rule_object {
    using ptr = std::shared_ptr<rule_object>;

    enum type {
        NONE, MATCH, REFERENCE, CONDITIONAL
    };

    type m_type = NONE;

    rule_object() = default;
    rule_object(const rule_object::type p_type) : m_type(p_type) { }
    virtual ~rule_object() = default;
};


struct rule_match : public rule_object {
    char m_symbol = 0;

    rule_match() : rule_object(rule_object::MATCH) { }
    virtual ~rule_match() = default;
};

struct rule_reference : public rule_object {
    std::vector<std::size_t> m_references;

    rule_reference() : rule_object(rule_object::REFERENCE) { }
    virtual ~rule_reference() = default;
};

struct rule_conditional : public rule_object {
    rule_reference m_rule_1;
    rule_reference m_rule_2;

    rule_conditional() : rule_object(rule_object::CONDITIONAL) { }
    virtual ~rule_conditional() = default;
};


using rule_collection = std::unordered_map<std::size_t, rule_object::ptr>;


struct task {
public:
    rule_collection m_rules;
    std::vector<std::string> m_expressions;

public:
    std::size_t match(const std::size_t p_index_rule) {
        std::size_t result = 0;
        for (auto & expr : m_expressions) {
            std::list<std::size_t> indexes = check(p_index_rule, expr, { 0 });

            for (auto index : indexes) {
                if (index == expr.size()) {
                    result++;
                    break;
                }
            }
        }

        return result;
    }

private:
    std::list<std::size_t> check(const std::size_t p_rule_index, const std::string p_line, const std::list<std::size_t> & p_indexes) {
        std::list<std::size_t> result = { };

        std::list<std::size_t> start_indexes;
        for (auto start_index : p_indexes) {
            if (start_index >= p_line.size()) {
                /* bad branch - ignore it */
            }
            else {
                start_indexes.push_back(start_index);
            }
        }

        if (start_indexes.empty()) {
            /* there no good starts - return nothing */
            return result;
        }

        auto p_rule = m_rules[p_rule_index];

        /* we might have several positive movements, but should check all of them */
        for (auto start_index : start_indexes) {

            switch (p_rule->m_type) {
            case rule_object::MATCH: {
                    auto rule = std::dynamic_pointer_cast<rule_match>(p_rule);
                    if (p_line[start_index] == rule->m_symbol) {
                        result.push_back(start_index + 1);
                    }

                    break;
                }

            case rule_object::REFERENCE: {
                    auto rule = std::dynamic_pointer_cast<rule_reference>(p_rule);

                    std::list<std::size_t> sub_result = { start_index };
                    for (auto reference : rule->m_references) {
                        sub_result = check(reference, p_line, sub_result);
                        if (sub_result.empty()) {
                            break;  /* bad branch */
                        }
                    }

                    result.merge(sub_result);
                    break;
                }

            case rule_object::CONDITIONAL: {
                    auto rule = std::dynamic_pointer_cast<rule_conditional>(p_rule);

                    std::list<std::size_t> sub_result = { start_index };
                    for (auto reference : rule->m_rule_1.m_references) {
                        sub_result = check(reference, p_line, sub_result);
                        if (sub_result.empty()) {
                            break;  /* bad branch */
                        }
                    }

                    result.merge(sub_result);

                    /* consider the second OR anyway */

                    sub_result = { start_index };
                    for (auto reference : rule->m_rule_2.m_references) {
                        sub_result = check(reference, p_line, sub_result);
                        if (sub_result.empty()) {
                            break;  /* bad branch */
                        }
                    }

                    result.merge(sub_result);
                    break;
                }

            default:
                throw std::invalid_argument("Unknown type of rule.");
            }
        }

        return result;
    }
};


rule_reference parse_reference(const std::string & p_values) {
    rule_reference result;

    std::size_t lborder = 0;
    std::size_t rborder = p_values.find(' ');
    while (rborder != std::string::npos) {
        auto value = p_values.substr(lborder, rborder - lborder);
        result.m_references.push_back(std::stoull(value));

        lborder = rborder + 1;
        rborder = p_values.find(' ', lborder);
    }

    auto value = p_values.substr(lborder);
    result.m_references.push_back(std::stoull(value));

    return result;
}


task read_task(const std::string & p_file) {
    const static std::regex pattern_conditional("(\\d+): (.*) \\| (.*)");
    const static std::regex pattern_match("(\\d+): \"(\\S)\"");
    const static std::regex pattern_reference("(\\d+): (.*)");

    task result;

    std::ifstream stream(p_file);

    bool is_rule_reading_state = true;
    for (std::string line; std::getline(stream, line); ) {
        if (line.empty()) {
            is_rule_reading_state = false;
            continue;
        }

        if (is_rule_reading_state) {
            std::smatch groups;
            if (std::regex_match(line, groups, pattern_conditional)) {
                std::size_t index = std::stoull(groups[1]);
                auto entry = std::make_shared<rule_conditional>();
                entry->m_rule_1 = parse_reference(groups[2]);
                entry->m_rule_2 = parse_reference(groups[3]);
                result.m_rules[index] = entry;
            }
            else if (std::regex_match(line, groups, pattern_match)) {
                auto entry = std::make_shared<rule_match>();

                std::size_t index = std::stoull(groups[1]);
                entry->m_symbol = std::string(groups[2])[0];
                result.m_rules[index] = entry;
            }
            else if (std::regex_match(line, groups, pattern_reference)) {
                std::size_t index = std::stoull(groups[1]);
                auto entry = std::make_shared<rule_reference>(parse_reference(groups[2]));
                result.m_rules[index] = entry;
            }
            else {
                throw std::invalid_argument("Incorrect input file.");
            }
        }
        else {
            result.m_expressions.push_back(line);
        }
    }

    return result;
}


int main() {
    task solver = read_task("input.txt");

    std::cout << "The amount of messages completely match rule 0: " << solver.match(0) << std::endl;

    std::cout << std::endl << "Updating rules..." << std::endl;
    rule_conditional rule_8, rule_11;

    rule_8.m_rule_1.m_references = { 42 };
    rule_8.m_rule_2.m_references = { 42, 8 };

    rule_11.m_rule_1.m_references = { 42, 31 };
    rule_11.m_rule_2.m_references = { 42, 11, 31 };

    solver.m_rules[8] = std::make_shared<rule_conditional>(rule_8);
    solver.m_rules[11] = std::make_shared<rule_conditional>(rule_11);

    std::cout << "The amount of messages completely match rule 0 (with loops): " << solver.match(0) << std::endl;

    return 0;
}
