#include <iostream>
#include <functional>
#include <fstream>
#include <list>
#include <numeric>
#include <sstream>
#include <string>


const std::int64_t OLD_VALUE = -1;
const std::int64_t INVALID_VALUE = -1;


struct action_t {
    std::int64_t item = INVALID_VALUE;
    std::int64_t index = INVALID_VALUE;

    bool no_action() { return item == INVALID_VALUE && index == INVALID_VALUE; }
};


struct operation_t {
    std::int64_t l = 0;
    std::int64_t r = 0;
    std::function<std::int64_t(std::int64_t, std::int64_t)> func;

    operation_t() = default;
    operation_t(const operation_t& p_op) = default;

public:
    std::int64_t eval(std::int64_t cur_level) {
        return func(get_value(l, cur_level), get_value(r, cur_level));
    }

private:
    std::int64_t get_value(std::int64_t value, std::int64_t cur_level) {
        return value == OLD_VALUE ? cur_level : value;
    }
};


class monkey_t {
private:
    std::list<std::int64_t> m_items;
    operation_t m_operation;
    std::int64_t m_divider = 0;
    std::int64_t m_index_true = 0;
    std::int64_t m_index_false = 0;
    std::uint64_t m_counter = 0;

public:
    monkey_t(const std::list<std::int64_t>& p_items, const operation_t& p_op, const std::int64_t divider, const std::int64_t index_true, const std::int64_t index_false) :
        m_items(p_items),
        m_operation(p_op),
        m_divider(divider),
        m_index_true(index_true),
        m_index_false(index_false)
    { }

public:
    action_t inspect(std::int64_t relief) {
        if (m_items.empty()) {
            return action_t{ };
        }

        m_counter++;

        std::int64_t item = m_items.front();
        m_items.pop_front();

        std::int64_t worry_level = m_operation.eval(item);
        std::int64_t new_value = worry_level / relief;

        std::int64_t index = (new_value % m_divider == 0) ? m_index_true : m_index_false;
        return { new_value, index };
    }

    void add(std::int64_t new_item) {
        m_items.push_back(new_item);
    }

    std::uint64_t get_counter() const {
        return m_counter;
    }

    std::int64_t get_divider() const {
        return m_divider;
    }
};


class solution {
private:
    std::vector<monkey_t> m_animals;
    std::int64_t m_normalizer = 0;

public:
    solution(const std::vector<monkey_t>& p_animals) : 
        m_animals(p_animals)
    {
        m_normalizer = 1;
        for (const auto& monkey : p_animals) {
            m_normalizer = std::lcm(m_normalizer, monkey.get_divider());
        }
    }

public:
    std::uint64_t monkey_business(std::int64_t n, std::int64_t relief) {
        for (std::int64_t i = 0; i < n; i++) {
            round(relief);
        }

        std::uint64_t first = 0, second = 0;
        for (const auto& monkey : m_animals) {
            const std::uint64_t counter = monkey.get_counter();
            if (first < counter) {
                second = first;
                first = counter;
            }
            else if (second < counter) {
                second = counter;
            }
        }

        return first * second;
    }

private:
    void round(std::int64_t relief) {
        for (auto & monkey : m_animals) {
            action_t action = monkey.inspect(relief);
            while (!action.no_action()) {
                action.item %= m_normalizer;
                m_animals[action.index].add(action.item);
                action = monkey.inspect(relief);
            }
        }
    }
};


std::vector<monkey_t> read_input() {
    std::fstream file("input.txt");

    std::vector<monkey_t> animals;
    for (std::string line; std::getline(file, line);) {
        if (line.empty()) {
            continue;
        }

        std::getline(file, line);
        std::stringstream ss(line);

        std::string ignore;
        ss >> ignore >> ignore;

        std::list<std::int64_t> items;
        for (std::int64_t value; ss >> value;) {
            ss >> ignore;
            items.push_back(value);
        }

        std::getline(file, line);
        ss = std::stringstream(line);

        ss >> ignore >> ignore >> ignore;
        char op;
        std::string l, r;
        ss >> l >> op >> r;

        operation_t operation;
        operation.l = (l == "old") ? OLD_VALUE : std::stoi(l);
        operation.r = (r == "old") ? OLD_VALUE : std::stoi(r);
        operation.func = (op == '+') ? [](std::int64_t l, std::int64_t r) { return l + r; } : [](std::int64_t l, std::int64_t r) { return l * r; };

        std::getline(file, line);
        ss = std::stringstream(line);
        ss >> ignore >> ignore >> ignore;

        std::int64_t divider = 0;
        ss >> divider;

        std::getline(file, line);
        ss = std::stringstream(line);
        ss >> ignore >> ignore >> ignore >> ignore >> ignore;
        
        std::int64_t index_true = 0;
        ss >> index_true;

        std::getline(file, line);
        ss = std::stringstream(line);
        ss >> ignore >> ignore >> ignore >> ignore >> ignore;

        std::int64_t index_false = 0;
        ss >> index_false;

        monkey_t m = { items, operation, divider, index_true, index_false };
        animals.push_back(m);
    }

    return animals;
}


int main() {
    auto input = read_input();

    std::cout << "The level of monkey business (20 rounds): " << solution(input).monkey_business(20, 3) << std::endl;
    std::cout << "The level of monkey business (10000 rounds, without relief): " << solution(input).monkey_business(10000, 1) << std::endl;

    return 0;
}