#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>


enum class command_type {
    CLEAR,
    SET,
    OR,
    AND
};


const std::unordered_map<std::string, command_type> from_string = {
    { "CLEAR", command_type::CLEAR },
    { "SET", command_type::SET },
    { "OR", command_type::OR },
    { "AND", command_type::AND }
};


struct command {
    command_type id;
    std::vector<std::uint64_t> arguments;
};


class solution {
public:
    std::string identify(const std::vector<command>& p_commands) {
        std::string result(32, '?');
        
        for (const auto& instruction : p_commands) {
            switch (instruction.id) {
                case command_type::CLEAR:
                    result[instruction.arguments[0]] = '0';
                    break;

                case command_type::SET:
                    result[instruction.arguments[0]] = '1';
                    break;

                case command_type::OR: {
                    const uint64_t idx1 = instruction.arguments[0];
                    const uint64_t idx2 = instruction.arguments[1];

                    if ((result[idx1] == '1') || (result[idx2] == '1')) {
                        result[idx1] = '1';
                    }
                    else if ((result[idx1] == '?' || result[idx2] == '?') && (result[idx1] == '0' || result[idx2] == '0')) {
                        result[idx1] = '?';
                    }

                    break;
                }

                case command_type::AND: {
                    const uint64_t idx1 = instruction.arguments[0];
                    const uint64_t idx2 = instruction.arguments[1];

                    if ((result[idx1] == '0') || (result[idx2] == '0')) {
                        result[idx1] = '0';
                    }
                    else if ((result[idx1] == '1') && (result[idx2] == '1')) {
                        result[idx1] = '1';
                    }
                    else if ((result[idx1] == '?' || result[idx2] == '?') && (result[idx1] == '1' || result[idx2] == '1')) {
                        result[idx1] = '?';
                    }

                    break;
                }
            }
        }
        
        std::reverse(result.begin(), result.end());
        return result;
    }
};


#include <iostream>


int main() {
    while (true) {
        int n;
        std::cin >> n;

        if (n == 0) {
            return 0;
        }

        std::vector<command> instructions;
        for (int i = 0; i < n; i++) {
            std::string name;
            std::cin >> name;

            command cmd;
            cmd.id = from_string.find(name)->second;

            int arg = 0;

            switch (cmd.id) {
            case command_type::CLEAR:
            case command_type::SET:
                std::cin >> arg;
                cmd.arguments.push_back(arg);
                break;

            case command_type::OR:
            case command_type::AND:
                std::cin >> arg;
                cmd.arguments.push_back(arg);

                std::cin >> arg;
                cmd.arguments.push_back(arg);
                break;
            }

            instructions.push_back(cmd);
        }

        std::string result = solution().identify(instructions);
        std::cout << result << std::endl;
    }

    return 0;
}