#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>


enum class shape {
    rock = 0,
    paper,
    scissors
};


const std::map<char, shape> TO_SHAPE = {
    { 'A', shape::rock },
    { 'X', shape::rock },
    { 'B', shape::paper },
    { 'Y', shape::paper },
    { 'C', shape::scissors },
    { 'Z', shape::scissors }
};


shape to_shape(const char p_value) {
    auto iter = TO_SHAPE.find(p_value);
    return iter->second;
}


const std::map<shape, int> SCORE_SHAPE = {
    { shape::rock, 1 },
    { shape::paper, 2 },
    { shape::scissors, 3 }
};


int to_score(const shape p_shape) {
    auto iter = SCORE_SHAPE.find(p_shape);
    return iter->second;
}


const std::map<shape, shape> SHAPE_WIN_MAP = {
    { shape::rock, shape::scissors },
    { shape::paper, shape::rock },
    { shape::scissors, shape::paper }
};


const std::map<shape, shape> SHAPE_LOSE_MAP = {
    { shape::scissors, shape::rock },
    { shape::rock, shape::paper },
    { shape::paper, shape::scissors }
};


bool is_winner(const shape p_first, const shape p_second) {
    auto iter = SHAPE_WIN_MAP.find(p_first);
    return iter->second == p_second;
}


shape get_winner(const shape p_other) {
    auto iter = SHAPE_WIN_MAP.find(p_other);
    return iter->second;
}


shape get_loser(const shape p_other) {
    auto iter = SHAPE_LOSE_MAP.find(p_other);
    return iter->second;
}


struct instruction {
    shape opponent;
    char me;

    instruction(const char p_opponent, const char p_me) {
        opponent = to_shape(p_opponent);
        me = p_me;
    }
};


std::vector<instruction> read() {
    std::ifstream file_stream("input.txt");

    std::vector<instruction> instructions;
    for (std::string line; std::getline(file_stream, line); ) {
        std::stringstream stream(line);

        char opponent, me;
        stream >> opponent >> me;

        instructions.emplace_back(opponent, me);
    }

    return instructions;
}


class game_analyser {
private:
    std::vector<instruction> m_instructions;

public:
    game_analyser(const std::vector<instruction>& p_instructions) :
        m_instructions(p_instructions)
    { }

public:
    int follow_assumed_guide() const {
        int result = 0;
        for (const auto& instr : m_instructions) {
            const auto me = to_shape(instr.me);
            result += to_score(me);

            if (me == instr.opponent) {
                result += 3;
            }
            else if (is_winner(me, instr.opponent)) {
                result += 6;
            }
            else {
                result += 0;
            }
        }

        return result;
    }

    int follow_elf_guide() const {
        int result = 0;
        for (const auto& instr : m_instructions) {
            switch (instr.me) {
            case 'X':
                result += to_score(get_winner(instr.opponent));
                break;

            case 'Y':
                result += to_score(instr.opponent);
                result += 3;
                break;

            case 'Z':
                result += to_score(get_loser(instr.opponent));
                result += 6;
                break;
            }
        }

        return result;
    }
};


#include <iostream>

int main() {
    const auto instructions = read();
    
    std::cout << "Outcome in case of following the guide with own assumption: " << game_analyser(instructions).follow_assumed_guide() << std::endl;
    std::cout << "Outcome in case of following the guide from elf: " << game_analyser(instructions).follow_elf_guide() << std::endl;

    return 0;
}