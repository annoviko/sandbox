#include <iostream>
#include <fstream>
#include <regex>
#include <list>
#include <string>
#include <vector>


struct instruction {
    int amount;
    int from;
    int to;
};


struct task {
    std::vector<std::list<char>> containers;
    std::vector<instruction> instructions;
};


task read_unloading_task() {
    task t;

    std::ifstream stream("input.txt");
    for (std::string line; std::getline(stream, line); ) {
        if (t.containers.empty()) {
            int n = (line.size() + 1) / 4;
            t.containers.resize(n);
        }

        if (line[1] == '1') {
            break;
        }

        line.push_back(' ');
        for (int i = 0; i < line.size() / 4; i++) {
            std::string block = line.substr(i * 4, 4);

            char item = block[1];
            if (item != ' ') {
                t.containers[i].push_front(item);
            }
        }
    }

    std::string ignore;
    std::getline(stream, ignore);

    for (std::string line; std::getline(stream, line); ) {
        std::regex pattern("move (\\d+) from (\\d+) to (\\d+)");
        std::smatch result;

        if (!std::regex_search(line, result, pattern)) {
            throw std::invalid_argument("incorrect instruction format");
        }

        instruction instr;
        instr.amount = std::stoi(result[1].str());
        instr.from = std::stoi(result[2].str()) - 1;
        instr.to = std::stoi(result[3].str()) - 1;

        t.instructions.push_back(instr);
    }

    return t;
}



class crate_mover_9000 {
protected:
    task m_task;

public:
    crate_mover_9000(const task& p_task) :
        m_task(p_task)
    { }

public:
    crate_mover_9000& execute() {
        for (const auto& in : m_task.instructions) {
            execute_instruction(in);
        }

        return *this;
    }

    std::string get_top_containers() const {
        std::string result;
        for (const auto& pile : m_task.containers) {
            if (pile.empty()) {
                result.push_back(' ');
            }
            else {
                result.push_back(pile.back());
            }
        }

        return result;
    }

protected:
    virtual void execute_instruction(const instruction& in) {
        for (int i = 0; i < in.amount; i++) {
            const char c = m_task.containers[in.from].back();
            m_task.containers[in.from].pop_back();

            m_task.containers[in.to].push_back(c);
        }
    }
};



class crate_mover_9001 : public crate_mover_9000 {
public:
    crate_mover_9001(const task& p_task) :
        crate_mover_9000(p_task)
    { }

protected:
    virtual void execute_instruction(const instruction& in) override {
        auto iter = m_task.containers[in.to].end();
        for (int i = 0; i < in.amount; i++) {
            const char c = m_task.containers[in.from].back();
            m_task.containers[in.from].pop_back();

            iter = m_task.containers[in.to].insert(iter, c);
        }
    }
};



int main() {
    task t = read_unloading_task();

    std::cout << "Top of each container (Create Mover 9000): " << crate_mover_9000(t).execute().get_top_containers() << std::endl;
    std::cout << "Top of each container (Create Mover 9001): " << crate_mover_9001(t).execute().get_top_containers() << std::endl;

    return 0;
}
