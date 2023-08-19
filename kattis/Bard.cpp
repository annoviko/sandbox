#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>


int main() {
    int n = 0;
    std::cin >> n;

    int events = 0;
    std::cin >> events;

    std::vector<std::unordered_set<int>> villagers(n);

    for (int i = 0; i < events; i++) {  // O(E)
        int k = 0;
        std::cin >> k;

        bool is_bard_here = false;
        std::vector<int> participants;
        for (int j = 0; j < k; j++) {
            int id = 0;
            std::cin >> id;
            id--;

            participants.push_back(id);
            if (id == 0) {
                is_bard_here = true;
            }
        }

        if (is_bard_here) {
            for (int id : participants) {
                villagers[id].insert(i);
            }
        }
        else {
            for (int signer : participants) {   // O(N)
                for (int song_id : villagers[signer]) { // O(S)
                    for (int listener : participants) { // O(N)
                        if (signer == listener) {
                            continue;
                        }

                        villagers[listener].insert(song_id); // O(1). Total -> O(N^2 * E * S)
                    }
                }
            }
        }
    }

    for (int i = 0; i < villagers.size(); i++) {
        if (villagers[i].size() == villagers[0].size()) {
            std::cout << i + 1 << std::endl;
        }
    }

    return 0;
}
