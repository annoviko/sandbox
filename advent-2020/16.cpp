#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <regex>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>


struct range {
public:
    std::size_t m_from = 0;
    std::size_t m_to = 0;

public:
    range() { }

    range(const std::size_t p_from, const std::size_t p_to) :
        m_from(p_from), m_to(p_to)
    { }

public:
    bool is_in(const std::size_t p_value) const {
        return (p_value >= m_from) && (p_value <= m_to);
    }
};


struct ticket_range {
public:
    std::string m_name;
    range       m_left;
    range       m_right;

public:
    ticket_range(const std::string & p_name, const range & p_left, const range & p_right) :
        m_name(p_name), m_left(p_left), m_right(p_right)
    { }

public:
    bool is_in(const std::size_t p_value) const {
        return m_left.is_in(p_value) || m_right.is_in(p_value);
    }
};


using range_container = std::vector<ticket_range>;

using ticket = std::vector<std::size_t>;
using ticket_container = std::list<ticket>;


class data {
private:
    range_container                                 m_ranges;
    std::vector<std::set<std::size_t>>    m_labels;

    ticket                      m_my;
    ticket_container            m_neighbors;

private:
    enum read_state {
        RANGE = 0, MY_TICKET, NEARBY_TICKET
    };

public:
    data(const std::string & p_file) : m_labels(1000) {
        std::ifstream stream(p_file);

        read_state state = RANGE;
        for (std::string line; std::getline(stream, line); ) {
            if (line.empty()) {
                state = static_cast<read_state>(static_cast<std::size_t>(state) + 1);
                std::getline(stream, line);
                continue;
            }

            switch (state) {
            case RANGE:
                fill_range_section(line);
                break;

            case MY_TICKET:
                m_my = read_ticket(line);
                break;

            case NEARBY_TICKET:
                m_neighbors.push_back(read_ticket(line));
                break;

            default:
                throw std::invalid_argument("Incorrect input data.");
            }
        }
    }

public:
    std::size_t find_invalid_neighbors() const {
        std::size_t result = 0;

        for (const auto & value : m_neighbors) {
            result += check_neighbor(value);
        }

        return result;
    }

    std::uint64_t discard_and_match(const std::string & p_prefix) {
        discard();

        std::vector<std::pair<std::set<std::size_t>, std::size_t>> match_fields;    /* keep match set and index-value-inp-ticket */
        for (std::size_t i = 0; i < m_my.size(); i++) {
            const std::size_t value = m_my[i];
            match_fields.push_back({ m_labels[value], i });
        }

        /* start filtering step by step */
        for (const auto & another_ticket : m_neighbors) {
            for (std::size_t index_field = 0; index_field < another_ticket.size(); index_field++) {
                const std::size_t value = another_ticket[index_field];

                auto & general_appearance = match_fields[index_field].first;
                auto current_appearance = m_labels[value];

                std::set<std::size_t> intersection;
                std::set_intersection(general_appearance.begin(), general_appearance.end(), current_appearance.begin(), current_appearance.end(), 
                    std::inserter(intersection, intersection.begin()));

                if (intersection.empty()) {
                    throw std::exception("At least one value should be in intersection.");
                }

                match_fields[index_field] = { intersection, match_fields[index_field].second };
            }
        }

        /* soft by size */
        std::sort(match_fields.begin(), match_fields.end(), 
            [](const std::pair<std::set<std::size_t>, std::size_t> & a, const std::pair<std::set<std::size_t>, std::size_t> & b)
            {
                return a.first.size() < b.first.size();
            }
        );

        /* fill my ticket */
        std::unordered_map<std::string, std::size_t> ticket_info;
        std::set<std::size_t> assigned_indexes; /* fill my ticket */

        for (std::size_t i = 0; i < match_fields.size(); i++) {
            auto current_appearance = match_fields[i];
            const std::size_t index_field = current_appearance.second;  /* index value in ticket */

            std::set<std::size_t> difference;
            std::set_difference(current_appearance.first.begin(), current_appearance.first.end(), assigned_indexes.begin(), assigned_indexes.end(),
                std::inserter(difference, difference.end()));

            if (difference.size() != 1) {
                throw std::invalid_argument("Ambiguous difference - algorithm should be improved.");
            }

            std::size_t index_ticket_entry = *(difference.begin()); /* index for ranges */
            assigned_indexes.insert(index_ticket_entry);

            if (!m_ranges[index_ticket_entry - 1].is_in(m_my[index_field])) {
                throw std::exception("Incorrect value is assigned.");
            }

            ticket_info[m_ranges[index_ticket_entry - 1].m_name] = m_my[index_field];
        }

        /* multiply */
        std::uint64_t result = 1;
        for (const auto & entry : ticket_info) {
            if (entry.first.size() < p_prefix.size()) {
                continue;
            }

            if (entry.first.substr(0, p_prefix.size()) != p_prefix) {
                continue;
            }

            result *= static_cast<std::uint64_t>(entry.second);
        }

        return result;
    }

private:
    void discard() {
        for (auto iter = m_neighbors.begin(); iter != m_neighbors.end();) {
            if (check_neighbor(*iter) > 0) {
                auto iter_to_remove = iter;
                iter++;
                m_neighbors.erase(iter_to_remove);
            }
            else {
                iter++;
            }
        }
    }


    std::size_t check_neighbor(const ticket & p_ticket) const {
        std::size_t result = 0;

        for (const std::size_t value : p_ticket) {
            if (m_labels[value].empty()) {
                result += value;
            }
        }

        return result;
    }


    void fill_labels(const std::size_t p_index, const std::size_t p_from, const std::size_t p_to) {
        for (std::size_t i = p_from; i <= p_to; i++) {
            m_labels[i].insert(p_index);
        }
    }


    void fill_range_section(const std::string & p_entry) {
        const static std::regex pattern("(.*): (\\d+)-(\\d+) or (\\d+)-(\\d+)");
        std::smatch results;

        if (!std::regex_match(p_entry, results, pattern)) {
            throw std::invalid_argument("Incorrect entry pattern.");
        }

        m_ranges.emplace_back(results[1], range(std::stoull(results[2]), std::stoull(results[3])), range(std::stoull(results[4]), std::stoull(results[5])));
        fill_labels(m_ranges.size(), m_ranges.back().m_left.m_from, m_ranges.back().m_left.m_to);
        fill_labels(m_ranges.size(), m_ranges.back().m_right.m_from, m_ranges.back().m_right.m_to);
    }


    ticket read_ticket(const std::string & p_entry) {
        ticket result;

        std::size_t lborder = 0;
        std::size_t rborder = p_entry.find(',');
        while (rborder != std::string::npos) {
            const std::string value = p_entry.substr(lborder, rborder - lborder);
            result.push_back(std::stoull(value));

            lborder = rborder + 1;
            rborder = p_entry.find(',', lborder);
        }

        const std::string value = p_entry.substr(lborder);
        result.push_back(std::stoull(value));

        return result;
    }
};


int main() {
    std::cout << "The ticket scanning error rate: " << data("input.txt").find_invalid_neighbors() << std::endl;
    std::cout << "The multiplication after matching: " << data("input.txt").discard_and_match("departure") << std::endl;

    return 0;
}
