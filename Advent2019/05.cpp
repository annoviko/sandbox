#include <fstream>
#include <iostream>
#include <list>
#include <vector>
#include <string>


std::vector<long> read_opcode() {
    std::vector<long> result;

    std::ifstream infile("input.txt");
    long value;
    while (infile >> value) {
        result.push_back(value);
        infile.ignore(1);
    }
    infile.close();

    return result;
}


class integer_computer {
private:
    struct instruction {
        long m_command;
        std::vector<long> m_mode;
    };

private:
    std::size_t m_instruction_counter;
    std::vector<long>& m_memory;

    std::list<long> m_input;
    std::list<long> m_output;

public:
    integer_computer(std::vector<long>& p_memory) :
        m_instruction_counter(0),
        m_memory(p_memory)
    { }

public:
    void execute() {
        for (std::size_t i = 0; i < m_memory.size();) {
            auto operation = get_instruction();

            switch (operation.m_command) {
            case 1: {
                add(operation);
                break;
            }

            case 2: {
                multiply(operation);
                break;
            }

            case 3: {
                write(operation);
                break;
            }

            case 4: {
                output(operation);
                break;
            }

            case 5: {
                jump_if_true(operation);
                break;
            }

            case 6: {
                jump_if_false(operation);
                break;
            }

            case 7: {
                less_than(operation);
                break;
            }

            case 8: {
                equals(operation);
                break;
            }

            case 99:
                return;

            default:
                throw std::exception(("Error: Unknown command '" + std::to_string(operation.m_command) 
                    + "' is specified (counter: '" + std::to_string(m_instruction_counter - 1) + "')!").c_str());
            }
        }

        throw std::exception("Error: Unexpected end of the program!");
    }

    std::list<long>& get_input(void) {
        return m_input;
    }

    std::list<long>& get_output(void) {
        return m_output;
    }

private:
    instruction get_instruction() {
        long code = m_memory[m_instruction_counter++];

        instruction result;
        result.m_command = code % 100;

        long code_mode = code / 100;

        result.m_mode = { 0, 0, 0 };
        for (std::size_t i = 0; code_mode > 0; i++) {
            result.m_mode[i] = code_mode % 10;
            code_mode /= 10;
        }

        return result;
    }

    long& get_ref_argument() {
        return m_memory[m_instruction_counter++];
    }

    long& get_ref_value(const long p_mode, long& p_code) {
        switch (p_mode) {
        case 0:
            return m_memory[p_code];
        case 1:
            return p_code;
        default:
            throw std::exception(std::string("Error: Unsupported mode is specified '" + std::to_string(p_mode) + "'.").c_str());
        }
    }

    void add(const instruction& operation) {
        auto value1 = get_ref_value(operation.m_mode[0], get_ref_argument());
        auto value2 = get_ref_value(operation.m_mode[1], get_ref_argument());
        auto & result = get_ref_value(operation.m_mode[2], get_ref_argument());

        result = value1 + value2;
    }

    void multiply(const instruction& operation) {
        auto value1 = get_ref_value(operation.m_mode[0], get_ref_argument());
        auto value2 = get_ref_value(operation.m_mode[1], get_ref_argument());
        auto & result = get_ref_value(operation.m_mode[2], get_ref_argument());

        result = value1 * value2;
    }

    void write(const instruction& operation) {
        if (operation.m_mode[0] == 1) {
            throw std::exception("Error: Immediate mode is forbidden for write operation.");
        }

        auto position_value = get_ref_argument();
        m_memory[position_value] = read_input();
    }

    void output(const instruction& operation) {
        auto value = get_ref_value(operation.m_mode[0], get_ref_argument());
        write_output(value);
    }

    void jump_if_true(const instruction& operation) {
        auto condition = get_ref_value(operation.m_mode[0], get_ref_argument());
        auto position = get_ref_value(operation.m_mode[1], get_ref_argument());

        if (condition) {
            m_instruction_counter = position;
        }
    }

    void jump_if_false(const instruction& operation) {
        auto condition = get_ref_value(operation.m_mode[0], get_ref_argument());
        auto position = get_ref_value(operation.m_mode[1], get_ref_argument());

        if (!condition) {
            m_instruction_counter = position;
        }
    }

    void less_than(const instruction& operation) {
        auto value1 = get_ref_value(operation.m_mode[0], get_ref_argument());
        auto value2 = get_ref_value(operation.m_mode[1], get_ref_argument());
        auto & result = get_ref_value(operation.m_mode[2], get_ref_argument());

        result = (value1 < value2) ? 1 : 0;
    }

    void equals(const instruction& operation) {
        auto value1 = get_ref_value(operation.m_mode[0], get_ref_argument());
        auto value2 = get_ref_value(operation.m_mode[1], get_ref_argument());
        auto& result = get_ref_value(operation.m_mode[2], get_ref_argument());

        result = (value1 == value2) ? 1 : 0;
    }

    void write_output(const long value) {
        m_output.push_back(value);
    }

    long read_input() {
        long value = m_input.front();
        m_input.erase(m_input.begin());
        return value;
    }
};



int main() {
    std::vector<long> memory = read_opcode();

    try {
        integer_computer computer(memory);
        computer.get_input().push_back(5);
        computer.execute();

        for (const auto value : computer.get_output()) {
            std::cout << value;
        }
        std::cout << std::endl;
    }
    catch (std::exception & error) {
        std::cout << error.what() << std::endl;
        return -1;
    }

    return 0;
}
