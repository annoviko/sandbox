#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <unordered_map>


struct candidate {
    std::string party;
    int votes = 0;
};


int main() {
    std::unordered_map<std::string, candidate> election;

    std::string line;
    std::getline(std::cin, line);

    int n = std::stoi(line);

    for (int i = 0; i < n; i++) {
        std::string name, party;

        std::getline(std::cin, name);
        std::getline(std::cin, party);

        election[name] = { party, 0 };
    }

    std::getline(std::cin, line);

    int m = std::stoi(line);

    for (int i = 0; i < m; i++) {
        std::string name;
        std::getline(std::cin, name);

        auto iter = election.find(name);
        if (iter != election.cend()) {
            election[name].votes++;
        }
    }

    std::string party_winner;
    int votes = 0;

    for (const auto& pair : election) {
        if (pair.second.votes > votes) {
            votes = pair.second.votes;
            party_winner = pair.second.party;
        }
        else if (votes == pair.second.votes) {
            party_winner.clear();
        }
    }

    if (party_winner.empty()) {
        std::cout << "tie" << std::endl;
    }
    else {
        std::cout << party_winner << std::endl;
    }

    return 0;
}