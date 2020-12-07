#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>


std::list<std::string> read_bag_rules() {
    std::ifstream stream("input.txt");

    std::list<std::string> result;

    for (std::string line; std::getline(stream, line); ) {
        result.push_back(line);
    }

    return result;
}


class transition_table {
private:
    struct destination_node {
    public:
        std::string m_name;
        std::size_t m_amount = 0;

    public:
        bool operator==(const destination_node & p_other) const {
            return m_name == p_other.m_name;
        }
    };

    using transition_table_t = std::unordered_multimap<std::string, destination_node>;

private:
    transition_table_t m_forward_table;
    transition_table_t m_backward_table;

public:
    transition_table(const std::list<std::string> & p_rules) {
        for (const auto & rule : p_rules) {
            parse_rule(rule);
        }
    }

    std::size_t get_minimum_bags_contain(const std::string & p_bag) {
        std::size_t result = 0;
        std::unordered_set<std::string> visited = { p_bag };

        for (std::unordered_set<std::string> to_process = { p_bag }; !to_process.empty(); ) {
            const std::string bag_name = (*to_process.begin());
            to_process.erase(bag_name);

            const auto bag_range = m_backward_table.equal_range(bag_name);
            for (auto iter_bag = bag_range.first; iter_bag != bag_range.second; iter_bag++) {
                if (visited.find(iter_bag->second.m_name) == visited.end()) {
                    to_process.insert(iter_bag->second.m_name);
                    visited.insert(iter_bag->second.m_name);
                    result++;
                }
            }
        }

        return result;
    }

    std::size_t get_required_bags_amount(const std::string & p_bag) {
        std::size_t result = 1;

        auto contained_bags = m_forward_table.equal_range(p_bag);
        for (auto iter_bag = contained_bags.first; iter_bag != contained_bags.second; iter_bag++) {
            if (iter_bag->second.m_name.empty()) {
                continue;
            }

            result += iter_bag->second.m_amount * get_required_bags_amount(iter_bag->second.m_name);
        }

        return result;
    }

private:
    void parse_rule(const std::string & p_rule) {
        std::regex pattern("(.*) bags contain (.*)");
        std::smatch groups;

        if (!std::regex_match(p_rule, groups, pattern)) {
            throw std::invalid_argument("Impossible to extract general pattern.");
        }

        const std::string node_name = groups[1];
        const std::string destination = groups[2];

        extract_destinations(node_name, destination);
    }


    void extract_destinations(const std::string & p_node_name, const std::string & p_destination) {
        if (p_destination == "no other bags.") {
            m_forward_table.insert({ p_node_name, { } });
            return;
        }

        std::size_t dest_position = 0;
        std::size_t comma_position = p_destination.find(',');
        while (comma_position != std::string::npos) {
            const std::string destination_node_rule = p_destination.substr(dest_position, comma_position - dest_position);
            destination_node dest_node = extract_destination_node(destination_node_rule);

            m_forward_table.insert({ p_node_name, dest_node });
            m_backward_table.insert({ dest_node.m_name, { p_node_name, 0 } });

            dest_position = comma_position + 2; /* coma + space */
            comma_position = p_destination.find(',', dest_position);
        }

        const std::string destination_node_rule = p_destination.substr(dest_position);
        destination_node dest_node = extract_destination_node(destination_node_rule);

        m_forward_table.insert({ p_node_name, dest_node });
        m_backward_table.insert({ dest_node.m_name, { p_node_name, 0 } });
    }


    destination_node extract_destination_node(const std::string & p_destination) {
        std::regex pattern("(\\d+) (.*) bag.*");
        std::smatch groups;

        if (!std::regex_match(p_destination, groups, pattern)) {
            throw std::invalid_argument("Impossible to extract destination pattern.");
        }

        return { groups[2], std::stoull(groups[1]) };
    }
};



int main() {
    const auto rules = read_bag_rules();
    transition_table table(rules);

    std::cout << "The amount of bag colors contain at least one 'shiny gold' bag: " << table.get_minimum_bags_contain("shiny gold") << std::endl;
    std::cout << "The total amount of required bags in case of 'shiny gold': " << table.get_required_bags_amount("shiny gold") - 1 << std::endl;

    return 0;
}
