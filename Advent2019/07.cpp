#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <numeric>
#include <vector>
#include <string>
#include <tuple>


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
        long m_command = 0;
        std::vector<long> m_mode = { };
    };

    enum class command_t {
        add = 1,
        multiply,
        write,
        output,
        jump_if_true,
        jump_if_false,
        less_than,
        equals,
        halt = 99
    };

public:
    enum class status {
        waiting_for_input,
        terminated,
    };

private:
    std::size_t m_instruction_counter;
    std::vector<long> m_memory;

    std::list<long> m_input;
    std::list<long> m_output;

public:
    integer_computer(std::vector<long>& p_memory) :
        m_instruction_counter(0),
        m_memory(p_memory)
    { }

public:
    status execute() {
        for (; m_instruction_counter < m_memory.size();) {
            auto operation = get_instruction();

            switch (static_cast<command_t>(operation.m_command)) {
            case command_t::add:
                add(operation);
                break;

            case command_t::multiply:
                multiply(operation);
                break;

            case command_t::write:
                if (m_input.empty()) {
                    m_instruction_counter -= 1;
                    return status::waiting_for_input;
                }
                write(operation);
                break;

            case command_t::output:
                output(operation);
                break;

            case command_t::jump_if_true:
                jump_if_true(operation);
                break;

            case command_t::jump_if_false:
                jump_if_false(operation);
                break;

            case command_t::less_than:
                less_than(operation);
                break;

            case command_t::equals:
                equals(operation);
                break;

            case command_t::halt:
                return status::terminated;

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
        auto& result = get_ref_value(operation.m_mode[2], get_ref_argument());

        result = value1 + value2;
    }

    void multiply(const instruction& operation) {
        auto value1 = get_ref_value(operation.m_mode[0], get_ref_argument());
        auto value2 = get_ref_value(operation.m_mode[1], get_ref_argument());
        auto& result = get_ref_value(operation.m_mode[2], get_ref_argument());

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
        auto& result = get_ref_value(operation.m_mode[2], get_ref_argument());

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


class amplifier {
private:
    long m_phase_setting = 0;
    std::vector<long> m_software;
    std::vector<long> m_memory;
    integer_computer m_computer;

public:
    amplifier(std::vector<long> & p_code) : 
        m_software(p_code),
        m_memory(p_code),
        m_computer(m_memory)
    { }

public:
    void set_phase(const long p_value) {
        m_phase_setting = p_value;
        m_computer.get_input().push_back(m_phase_setting);
    }

    void restart() {
        m_memory = m_software;
        m_computer = integer_computer(m_memory);
    }

    std::tuple<long, integer_computer::status> run(const long p_input) {
        m_computer.get_input().push_back(p_input);

        auto result = m_computer.execute();

        return { m_computer.get_output().back(), result };
    }
};


class energy_optimizer {
private:
    std::vector<amplifier> m_chain;

public:
    energy_optimizer(
        std::size_t p_amount_amplifiers,
        std::vector<long>& amplifier_controller_software) :
            m_chain(p_amount_amplifiers, amplifier(amplifier_controller_software))
    { }

public:
    long optimize(bool has_feedback_loop = false) {
        long maximum_enegry = 0;
        long setting_start = has_feedback_loop ? 5 : 0;

        std::vector<long> amplifier_settings(m_chain.size());
        std::iota(amplifier_settings.begin(), amplifier_settings.end(), setting_start);

        do {
            long amplifier_output = run_chain(0, amplifier_settings);

            std::for_each(m_chain.begin(), m_chain.end(), [](auto& device) { device.restart(); });

            if (amplifier_output > maximum_enegry) {
                maximum_enegry = amplifier_output;
            }
        } while (std::next_permutation(amplifier_settings.begin(), amplifier_settings.end()));

        return maximum_enegry;
    }

private:
    long run_chain(long amplifier_input, std::vector<long>& p_amplifier_settings) {
        for (std::size_t i = 0; i < m_chain.size(); i++) {
            m_chain[i].set_phase(p_amplifier_settings[i]);
        }

        bool continue_processing = false;
        do {
            continue_processing = false;
            for (std::size_t i = 0; i < m_chain.size(); i++) {
                auto result = m_chain[i].run(amplifier_input);

                amplifier_input = std::get<0>(result);
                auto status = std::get<1>(result);

                continue_processing |= (status == integer_computer::status::waiting_for_input);
            }
        } while (continue_processing);

        return amplifier_input;
    }
};


int main() {
    std::vector<long> software_program = read_opcode();

    try {
        energy_optimizer optimizer(5, software_program);
        std::cout << "The highest signal that can be sent to the thrusters: " << optimizer.optimize() << std::endl;
        std::cout << "The highest signal that can be sent to the thrusters (feedback loop): " << optimizer.optimize(true) << std::endl;
    }
    catch (std::exception & error) {
        std::cout << error.what() << std::endl;
        return -1;
    }

    return 0;
}