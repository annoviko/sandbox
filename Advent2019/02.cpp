#include <fstream>
#include <iostream>
#include <cstdint>
#include <vector>
#include <tuple>


std::vector<std::size_t> read_opcode() {
    std::vector<std::size_t> result;

    std::ifstream infile("input.txt");
    std::size_t value;
    while (infile >> value) {
        result.push_back(value);
        infile.ignore(1);
    }
    infile.close();

    return result;
}


std::size_t restore_state(std::vector<std::size_t> & p_opcode) {
    for (std::size_t i = 0; i < p_opcode.size(); i += 4) {
        auto operation = p_opcode[i];
        auto position_value1 = p_opcode[i + 1];
        auto position_value2 = p_opcode[i + 2];
        auto position_result = p_opcode[i + 3];

        switch (operation) {
        case 1:
            p_opcode[position_result] = p_opcode[position_value1] + p_opcode[position_value2];
            break;
        case 2:
            p_opcode[position_result] = p_opcode[position_value1] * p_opcode[position_value2];
            break;
        case 99:
            return p_opcode[0];
        default:
            std::cout << "Error: failure input or processing!" << std::endl;
            std::exit(-1);
        }
    }

    std::exit(-1);
}


#if BRUTE_FORCE_SOLUTION
int main() {
    auto opcode = read_opcode();
    auto original = opcode;

    for (std::size_t noun = 0; noun < 100; noun++) {
        for (std::size_t verb = 0; verb < 100; verb++) {
            opcode = original;

            opcode[1] = noun;
            opcode[2] = verb;

            std::size_t state = restore_state(opcode);

            std::cout << "State before alarm: " << opcode[0] << std::endl;

            if (state == 19690720) {
                std::cout << "100 * noun + verb: " << noun * 100 + verb << std::endl;
                return 0;
            }
        }
    }

    return 0;
}
#else


const std::size_t state_to_search = 19690720;


std::size_t binary_search_verb(const std::vector<std::size_t> & original, const std::size_t noun) {
    auto opcode = original;
    std::size_t left_verb = 0, right_verb = 99;

    while (left_verb < right_verb) {
        opcode = original;

        std::size_t verb = (right_verb + left_verb) / 2;

        opcode[1] = noun;
        opcode[2] = verb;

        std::size_t state = restore_state(opcode);

        std::cout << "State before alarm: " << opcode[0] << std::endl;
        if (state > state_to_search) {
            right_verb = verb - 1;
        }
        else if(state < state_to_search) {
            left_verb = verb + 1;
        }
        else {
            return verb;
        }
    }

    throw std::exception("State is not found!");
}

std::size_t binary_search_noun(const std::vector<std::size_t>& original, const std::size_t verb) {
    auto opcode = original;

    std::size_t left_noun = 0, right_noun = 99;
    std::size_t last_lower_noun = -1;

    while (left_noun < right_noun) {
        opcode = original;

        std::size_t noun = (left_noun + right_noun) / 2;

        opcode[1] = noun;
        opcode[2] = verb;

        std::size_t state = restore_state(opcode);

        std::cout << "State before alarm: " << opcode[0] << std::endl;
        if (state > state_to_search) {
            right_noun = noun - 1;
        }
        else if (state < state_to_search) {
            left_noun = noun + 1;
            last_lower_noun = noun;
        }
        else {
            return noun;
        }
    }

    return last_lower_noun;
}

int main() {
    auto opcode = read_opcode();
    auto original = opcode;

    std::size_t noun = binary_search_noun(opcode, 0);
    std::size_t verb = binary_search_verb(opcode, noun);

    std::cout << "100 * noun + verb: " << noun * 100 + verb << std::endl;

    return 0;
}
#endif