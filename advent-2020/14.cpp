#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <variant>
#include <unordered_map>


struct update_mask {
public:
    std::uint64_t m_set_mask = 0;
    std::uint64_t m_clr_mask = 0;
    std::string m_raw;
};


struct write_memory {
public:
    std::uint64_t m_addr = 0;
    std::uint64_t m_value = 0;
};


using instruction = std::variant<update_mask, write_memory>;
using instruction_sequence = std::vector<instruction>;

using memory = std::unordered_map<std::uint64_t, std::uint64_t>;


class emulator_version_1 {
protected:
    std::uint64_t m_set_mask = 0;
    std::uint64_t m_clr_mask = 0;

    memory m_memory;

public:
    emulator_version_1 & process(const instruction_sequence & p_instruction) {
        for (const auto & to_execute : p_instruction) {
            process(to_execute);
        }

        return *this;
    }

    std::uint64_t compute_sum() const {
        std::uint64_t result = 0;
        for (const auto & entry : m_memory) {
            result += entry.second;
        }

        return result;
    }

public:
    virtual void operator()(const update_mask & p_instruction) {
        m_set_mask = p_instruction.m_set_mask;
        m_clr_mask = p_instruction.m_clr_mask;
    }

    virtual void operator()(const write_memory & p_instruction) {
        std::uint64_t value = p_instruction.m_value | m_set_mask;
        value &= m_clr_mask;

        m_memory[p_instruction.m_addr] = value;
    }

private:
    void process(const instruction & p_instruction) {
        std::visit(*this, p_instruction);
    }
};


class address_generator {
private:
    std::uint64_t m_fixed_mask = 0;

    std::uint64_t m_counter = 0;
    std::uint64_t m_limit = 0;

    std::vector<char> m_positions;

public:
    address_generator() { }

    address_generator(const std::string & p_mask) {
        init(p_mask);
    }

public:
    void reset() {
        m_counter = 0;
    }

    void init(const std::string & p_mask) {
        m_counter = 0;
        m_fixed_mask = 0;
        m_positions.clear();

        for (auto iter = p_mask.rbegin(); iter != p_mask.rend(); iter++) {
            std::uint64_t position = iter - p_mask.rbegin();

            switch (*iter) {
            case 'X':
                m_positions.push_back(position);
                break;
            case '1':
                m_fixed_mask |= (std::uint64_t(1) << position);
                break;
            }
        }

        m_limit = (std::uint64_t(1) << m_positions.size());
    }

    std::uint64_t generate(const std::uint64_t p_value) {
        if (m_counter >= m_limit) {
            return -1;
        }

        std::uint64_t result = p_value | m_fixed_mask;

        std::uint64_t reminder = m_counter;
        for (auto bit_index : m_positions) {
            if ((reminder & 1) == 1) {
                result |= (std::uint64_t(1) << bit_index);      /* set bit */
            }
            else {
                result &= ~(std::uint64_t(1) << bit_index);     /* clear bit */
            }

            reminder >>= 1;
        }

        m_counter++;

        return result;
    }
};


class emulator_version_2 : public emulator_version_1 {
private:
    std::string       m_raw_mask;
    address_generator m_generator;

public:
    emulator_version_2 & process(const instruction_sequence & p_instruction) {
        for (const auto & to_execute : p_instruction) {
            process(to_execute);
        }

        return *this;
    }

public:
    virtual void operator()(const update_mask & p_instruction) override {
        m_raw_mask = p_instruction.m_raw;
        m_generator.init(m_raw_mask);
    }

    virtual void operator()(const write_memory & p_instruction) override {
        m_generator.reset();
        auto address = m_generator.generate(p_instruction.m_addr);
        while (address != (std::uint64_t) -1) {
            m_memory[address] = p_instruction.m_value;
            address = m_generator.generate(p_instruction.m_addr);
        }
    }

private:
    void process(const instruction & p_instruction) {
        std::visit(*this, p_instruction);
    }
};



instruction_sequence read_instructions(const std::string & p_file) {
    std::ifstream stream(p_file);

    instruction_sequence result;

    for (std::string line; std::getline(stream, line); ) {
        std::regex pattern_update_mask("mask = (\\S+)");
        std::regex pattern_write_memory("mem\\[(\\d+)\\] = (\\d+)");
        std::smatch groups;

        if (std::regex_match(line, groups, pattern_update_mask)) {
            update_mask instruction;

            std::string string_mask = groups[1];
            for (auto iter = std::begin(string_mask); iter != std::end(string_mask); iter++) {
                instruction.m_set_mask <<= 1;
                instruction.m_clr_mask <<= 1;

                switch (*iter) {
                case '0':
                    instruction.m_clr_mask |= 1;
                    break;
                case '1':
                    instruction.m_set_mask |= 1;
                    break;
                case 'X':
                    break;
                }
            }

            instruction.m_clr_mask ^= static_cast<std::uint64_t>(-1);
            instruction.m_raw = string_mask;
            result.push_back(instruction);
        }
        else if (std::regex_match(line, groups, pattern_write_memory)) {
            write_memory instruction;

            instruction.m_addr = std::stoull(groups[1]);
            instruction.m_value = std::stoull(groups[2]);

            result.push_back(instruction);
        }
        else {
            throw std::invalid_argument("Unknown pattern in the input file.");
        }
    }

    return result;
}



int main() {
    const auto instructions = read_instructions("input.txt");

    std::cout << "The sum of all values left in memory after it completes (version 1): " << emulator_version_1().process(instructions).compute_sum() << std::endl;
    std::cout << "The sum of all values left in memory after it completes (version 2): " << emulator_version_2().process(instructions).compute_sum() << std::endl;

    return 0;
}
