#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>


struct task {
    int from;
    int to;
};


std::vector<std::pair<task, task>> read_tasks() {
    std::vector<std::pair<task, task>> clean_tasks;

    std::ifstream stream("input.txt");
    for (std::string line; std::getline(stream, line); ) {
        std::regex pattern("(\\d+)-(\\d+),(\\d+)-(\\d+)");
        std::smatch result;

        if (!std::regex_search(line, result, pattern)) {
            throw std::invalid_argument("incorrect file format");
        }
        
        task t1, t2;
        t1.from = std::stoi(result[1].str());
        t1.to = std::stoi(result[2].str());
        t2.from = std::stoi(result[3].str());
        t2.to = std::stoi(result[4].str());

        clean_tasks.push_back({ t1, t2 });
    }

    return clean_tasks;
}


class task_analyser {
private:
    std::vector<std::pair<task, task>> m_tasks;

public:
    task_analyser(const std::vector<std::pair<task, task>>& p_tasks) :
        m_tasks(p_tasks)
    { }

public:
    int count_fully_contained_tasks() {
        int counter = 0;
        for (const auto& pair : m_tasks) {
            if (is_fully_contained(pair)) {
                counter++;
            }
        }

        return counter;
    }

    int count_overlapped_tasks() {
        int counter = 0;
        for (const auto& pair : m_tasks) {
            if (is_overlapped(pair)) {
                counter++;
                //std::cout << "overlap: (" << pair.first.from << ", " << pair.first.to << ") and (" << pair.second.from << ", " << pair.second.to << ")" << std::endl;
            }
        }

        return counter;
    }

private:
    bool is_fully_contained(const std::pair<task, task>& p_task) {
        const auto& t1 = p_task.first;
        const auto& t2 = p_task.second;

        return is_1_fully_contained_2(t1, t2) || is_1_fully_contained_2(t2, t1);
    }

    bool is_1_fully_contained_2(const task& t1, const task& t2) {
        return ((t1.from >= t2.from) && (t1.to <= t2.to));
    }

    bool is_overlapped(const std::pair<task, task>& p_task) {
        const auto& t1 = p_task.first;
        const auto& t2 = p_task.second;

        return is_point_inside(t1.from, t2) || is_point_inside(t1.to, t2) || is_fully_contained(p_task);
    }

    bool is_point_inside(const int p, const task& t) {
        return ((p >= t.from) && (p <= t.to));
    }
};


int main() {
    const auto tasks = read_tasks();

    std::cout << "Fully contained tasks: " << task_analyser(tasks).count_fully_contained_tasks() << std::endl;
    std::cout << "Overlapped tasks: " << task_analyser(tasks).count_overlapped_tasks() << std::endl;

    return 0;
}
