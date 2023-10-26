#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <regex>
#include <string>
#include <unordered_map>
#include <stack>


class instruction {
public:
    enum type {
        acc,
        jmp,
        nop
    };

private:
    const static std::regex                             BASIC_PATTERN;
    const static std::unordered_map<std::string, type>  TYPE_MAP;

private:
    type    m_command;
    int     m_argument;

public:
    instruction(const std::string & p_instruction) {
        std::smatch groups;

        if (!std::regex_match(p_instruction, groups, BASIC_PATTERN)) {
            throw std::invalid_argument("Incorrect instruction is provided.");
        }

        m_command = TYPE_MAP.at(groups[1]);
        m_argument = std::stoi(groups[2]);
    }

public:
    void set_type(const type & p_type) { m_command = p_type; }
    type get_type() const { return m_command; }
    int get_argument() const { return m_argument; }
};


const std::regex instruction::BASIC_PATTERN("(\\S+) ([+-]\\d+)");
const std::unordered_map<std::string, instruction::type> instruction::TYPE_MAP = {
    { "acc", instruction::acc },
    { "jmp", instruction::jmp },
    { "nop", instruction::nop }
};


using advent_program_code = std::vector<instruction>;
using advent_code_statistic = std::vector<std::size_t>;


class advent_program_loader {
public:
    static advent_program_code load(const std::string & p_file) {
        std::ifstream stream(p_file);

        advent_program_code result;
        for (std::string line; std::getline(stream, line); ) {
            result.push_back(instruction(line));
        }

        return result;
    }
};


class advent_computer {
public:
    enum status {
        initial,
        success,
        failure
    };

    struct execution_result {
        status      m_status = initial;
        std::size_t m_code   = 0;
    };

private:
    int                     m_accumulator = 0;
    std::size_t             m_program_counter = 0;
    advent_program_code     m_code;
    advent_code_statistic   m_stats;
    status                  m_exit_status = initial;

public:
    advent_computer(const advent_program_code & p_code) :
        m_code(p_code),
        m_stats(p_code.size(), 0)
    { }

    advent_computer(const advent_program_code & p_code, const int p_accumulator, const std::size_t p_program_counter, const advent_code_statistic & p_stats) :
        m_accumulator(p_accumulator),
        m_program_counter(p_program_counter),
        m_code(p_code),
        m_stats(p_stats)
    { }

public:
    void run() {
        while ((m_program_counter < m_code.size()) && (m_stats[m_program_counter] == 0)) {
            execute();
        }

        if (m_program_counter >= m_code.size()) {
            m_exit_status = status::success;
        }
        else {
            m_exit_status = status::failure;
        }
    }

    void run_auto_corrected() {
        std::stack<std::size_t> instructions;

        while (m_stats[m_program_counter] == 0) {
            instructions.push(m_program_counter);
            execute();
        }

        status exit_status = status::failure;
        while (exit_status != status::success) {
            m_program_counter = instructions.top();
            instructions.pop();

            revert_execution();
            exit_status = try_correct_instruction();
        }
    }

    int get_accumulator() const {
        return m_accumulator;
    }

    status get_exit_status() const {
        return m_exit_status;
    }

private:
    bool invert_instruction() {
        switch (m_code[m_program_counter].get_type()) {
        case instruction::jmp:
            m_code[m_program_counter].set_type(instruction::nop);
            return true;
        case instruction::nop:
            m_code[m_program_counter].set_type(instruction::jmp);
            return true;
        default:
            return false;
        }
    }

    status try_correct_instruction() {
        if (invert_instruction()) {
            advent_computer execution_instance(m_code, m_accumulator, m_program_counter, m_stats);
            execution_instance.run();

            if (execution_instance.get_exit_status() == status::success) {
                m_accumulator = execution_instance.get_accumulator();
            }

            invert_instruction();
            return execution_instance.get_exit_status();
        }

        return status::failure;
    }

    void revert_execution() {
        const instruction & command = m_code[m_program_counter];
        m_stats[m_program_counter]--;

        switch (command.get_type()) {
        case instruction::acc:
            m_accumulator -= command.get_argument();
            break;

        case instruction::jmp:
        case instruction::nop:
            break;

        default:
            throw std::invalid_argument("Unknow instruction for the avent computer");
        }
    }

    void execute() {
        const instruction & command = m_code[m_program_counter];
        m_stats[m_program_counter]++;

        switch (command.get_type()) {
        case instruction::acc:
            m_accumulator += command.get_argument();
            m_program_counter++;
            break;

        case instruction::jmp:
            m_program_counter += command.get_argument();
            break;

        case instruction::nop:
            m_program_counter++;
            break;

        default:
            throw std::invalid_argument("Unknow instruction for the avent computer");
        }
    }
};


int main() {
    advent_program_code code = advent_program_loader::load("input.txt");

    {
        advent_computer computer(code);
        computer.run();

        std::cout << "Value of the accumulator: " << computer.get_accumulator() << std::endl;
    }

    {
        advent_computer computer(code);
        computer.run_auto_corrected();

        std::cout << "Value of the accumulator after termination: " << computer.get_accumulator() << std::endl;
    }

    return 0;
}
