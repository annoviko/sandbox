#include <iostream>
#include <string>
#include <unordered_map>


struct result {
    bool m_done = false;
    std::size_t m_score = 0;
};


int main() {
    std::unordered_map<char, result> tasks;

    while (true) {
        int minutes = 0;
        std::cin >> minutes;

        if (minutes == -1) {
            break;
        }

        char task_id = 0;
        std::cin >> task_id;

        std::string verdict;
        std::cin >> verdict;

        tasks[task_id].m_done = (verdict == "right");
        if (!tasks[task_id].m_done) {
            tasks[task_id].m_score += 20;
        }
        else {
            tasks[task_id].m_score += minutes;
        }
    }

    int score = 0;
    int amount = 0;
    for (const auto& task : tasks) {
        if (task.second.m_done) {
            amount += 1;
            score += task.second.m_score;
        }
    }

    std::cout << amount << " " << score << std::endl;

    return 0;
}