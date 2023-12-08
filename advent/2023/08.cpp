#include <iostream>
#include <fstream>
#include <numeric>
#include <regex>
#include <string>
#include <unordered_map>


struct entry {
    std::string left;
    std::string right;
};


struct pouche {
    std::string instruction;
    std::unordered_map<std::string, entry> map;
};


class wasteland {
private:
    std::string instruction;
    std::unordered_map<std::string, entry> map;

public:
    wasteland(const pouche& p) :
        instruction(p.instruction),
        map(p.map)
    { }

public:
    std::uint64_t count_steps_to_escape() {
        std::uint64_t cur_instr = 0;
        std::uint64_t steps_to_escape = 0;

        std::string key = "AAA";
        for (; key != "ZZZ"; cur_instr++, steps_to_escape++) {
            if (cur_instr >= instruction.size()) {
                cur_instr = 0;
            }

            const char step = instruction[cur_instr];
            const auto iter = map.find(key);
            key = (step == 'L') ? iter->second.left : iter->second.right;
        }

        return steps_to_escape;
    }


    std::uint64_t count_steps_to_escape_as_ghost() {
        std::vector<std::string> positions;
        for (auto& iter : map) {
            if (iter.first.back() == 'A') {
                positions.push_back(iter.first);
            }
        }

        std::vector<std::uint64_t> steps_to_reach_z;
        for (auto& initial_pos : positions) {
            std::string position = initial_pos;
            std::uint64_t steps = 0;
            std::uint64_t cur_instr = 0;

            while (position.back() != 'Z') {
                if (cur_instr >= instruction.size()) {
                    cur_instr = 0;
                }

                const auto iter = map.find(position);
                position = (instruction[cur_instr] == 'L') ? iter->second.left : iter->second.right;

                steps++;
                cur_instr++;
            }

            steps_to_reach_z.push_back(steps);
        }

        /* least common multiple among all of them */
        std::uint64_t lcm = steps_to_reach_z[0];
        for (std::uint64_t i = 1; i < steps_to_reach_z.size(); i++) {
            lcm = std::lcm(lcm, steps_to_reach_z[i]);
        }

        return lcm;
    }
};


pouche retrieve() {
    pouche result;

    std::fstream f("input.txt");

    std::string line;
    std::getline(f, result.instruction);

    std::getline(f, line);
    std::regex pattern("(\\S+) = \\((\\S+), (\\S+)\\)");
    while (std::getline(f, line)) {
        std::smatch match_result;
        std::regex_search(line, match_result, pattern);

        std::string key, left, right;
        key = match_result[1].str();
        left = match_result[2].str();
        right = match_result[3].str();

        result.map[key] = { left, right };
    }

    return result;
}


int main() {
    auto bag = retrieve();

    std::cout << "Steps required to reach ZZZ: " << wasteland(bag).count_steps_to_escape() << std::endl;
    std::cout << "Steps required to reach Z as a ghost: " << wasteland(bag).count_steps_to_escape_as_ghost() << std::endl;

    return 0;
}
