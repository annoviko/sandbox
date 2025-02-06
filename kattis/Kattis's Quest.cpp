#include <iostream>
#include <queue>
#include <map>
#include <string>
#include <vector>


int main() {
    std::map<std::int64_t, std::priority_queue<std::int64_t>> quests;   /* energy -> [rewards..] */

    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::string command;
        std::cin >> command;

        if (command == "add") {
            std::int64_t energy, reward;
            std::cin >> energy >> reward;

            quests[energy].push(reward);
        }
        else {
            std::int64_t energy;
            std::cin >> energy;

            bool quest_done = true;
            std::int64_t reward = 0;

            while (quest_done && (energy > 0)) {
                quest_done = false;

                auto iter = quests.upper_bound(energy);
                if (iter != quests.begin()) {
                    iter--;

                    reward += iter->second.top();
                    energy -= iter->first;

                    iter->second.pop();
                    if (iter->second.empty()) {
                        quests.erase(iter);
                    }

                    quest_done = true;
                }
            }

            std::cout << reward << std::endl;
        }
    }

    return 0;
}