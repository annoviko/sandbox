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
};


struct write_memory {
public:
    std::uint64_t m_addr = 0;
    std::uint64_t m_value = 0;
};


using instruction = std::variant<update_mask, write_memory>;
using instruction_sequence = std::vector<instruction>;

using memory = std::unordered_map<std::uint64_t, std::uint64_t>;


class emulator {
private:
    std::uint64_t m_set_mask;
    std::uint64_t m_clr_mask;
    memory m_memory;

public:
    emulator & process(const instruction_sequence & p_instruction) {
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
    void operator()(const update_mask & p_instruction) {
        m_set_mask = p_instruction.m_set_mask;
        m_clr_mask = p_instruction.m_clr_mask;
    }

    void operator()(const write_memory & p_instruction) {
        std::uint64_t value = p_instruction.m_value | m_set_mask;
        value &= m_clr_mask;

        m_memory[p_instruction.m_addr] = value;
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

    std::cout << "The sum of all values left in memory after it completes: " << emulator().process(instructions).compute_sum() << std::endl;

    return 0;
}
