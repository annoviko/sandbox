#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stack>


std::vector<std::string> read_expressions(const std::string & p_file) {
    std::vector<std::string> result;

    std::ifstream stream(p_file);
    for (std::string line; std::getline(stream, line); ) {
        result.push_back(line);
    }

    return result;
}


class solver {
private:
    std::stack<std::int64_t> m_stack;
    std::string              m_output;
    bool                     m_priority;

    std::string              m_expression;

public:
    solver(const std::string & p_expression, const bool p_priority) : 
        m_expression(p_expression), m_priority(p_priority)
    {
        build_output();
        evaluate();
    }

public:
    std::int64_t get() const {
        return m_stack.top();
    }

private:
    bool is_operation(const char p_symbol) const {
        switch (p_symbol) {
            case '+':
            case '*':
                return true;
            default:
                return false;
        }
    }

    bool is_higher_equal_priority(const char p_left, const char p_right) const {
        if (p_left == '+') {
            return true;    /* left is higher or equal */
        }
        else {
            if (p_right == '+') {
                return false;   /* left is less to right */
            }
            else {
                return true;    /* left is equal to right */
            }
        }
    }

    void build_output() {
        const auto no_priority_proc = [this](const char symbol) { 
            (void)symbol;

            while (!m_stack.empty() && is_operation(static_cast<char>(m_stack.top()))) {
                m_output.push_back(static_cast<char>(m_stack.top()));
                m_stack.pop();
            }
        };

        const auto priority_proc = [this](const char symbol) {
            while (!m_stack.empty() && is_operation(static_cast<char>(m_stack.top())) && is_higher_equal_priority(static_cast<char>(m_stack.top()), symbol)) {
                m_output.push_back(static_cast<char>(m_stack.top()));
                m_stack.pop();
            }
        };

        for (char symbol : m_expression) {
            switch (symbol) {
            case ' ':
                break;

            case '+':
            case '*':
                if (m_priority) {
                    priority_proc(symbol);
                }
                else {
                    no_priority_proc(symbol);
                }

                m_stack.push(symbol);
                break;

            case '(':
                m_stack.push(symbol);
                break;

            case ')':
                while (m_stack.top() != '(') {
                    m_output.push_back(static_cast<char>(m_stack.top()));
                    m_stack.pop();
                }

                m_stack.pop();  /* remove bracket */
                break;

            default:
                m_output.push_back(static_cast<char>(symbol));
                break;
            }
        }

        while (!m_stack.empty()) {
            m_output.push_back(static_cast<char>(m_stack.top()));
            m_stack.pop();
        }
    }

    void evaluate() {
        for (char symbol : m_output) {
            if (is_operation(symbol)) {
                std::int64_t op1 = m_stack.top();
                m_stack.pop();

                std::int64_t op2 = m_stack.top();
                m_stack.pop();

                switch (symbol) {
                case '+':
                    m_stack.push(op1 + op2);
                    break;

                case '*':
                    m_stack.push(op1 * op2);
                    break;

                default:
                    throw std::invalid_argument("Operation is not supported.");
                }
            }
            else {
                m_stack.push(symbol - 48);
            }
        }
    }
};


std::int64_t sum_expression_outputs(std::vector<std::string> & p_container, const bool p_priority) {
    std::int64_t result = 0;

    for (auto & expr : p_container) {
        result += solver(expr, p_priority).get();
    }

    return result;
}


int main() {
    auto content = read_expressions("input.txt");

    std::cout << "The sum of the resulting values: " << sum_expression_outputs(content, false) << std::endl;
    std::cout << "The sum of the resulting values with priorities: " << sum_expression_outputs(content, true) << std::endl;

    return 0;
}
