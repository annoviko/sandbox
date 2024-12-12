#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <functional>


struct pokemon {
    std::uint64_t id;
    std::uint64_t attack;
    std::uint64_t defense;
    std::uint64_t health;
};


void select(std::uint64_t k, std::vector<pokemon>& g, const std::function<bool(const pokemon&, const pokemon&)>& comp, std::unordered_set<std::uint64_t>& chosen) {
    std::sort(g.begin(), g.end(), comp);

    for (std::uint64_t i = 0; i < k; i++) {
        chosen.insert(g[i].id);
    }
}


int main() {
    int n, k;
    std::cin >> n >> k;

    std::unordered_set<std::uint64_t> chosen;

    std::vector<pokemon> group;
    for (std::uint64_t i = 0; i < n; i++) {
        pokemon p;
        p.id = i;

        std::cin >> p.attack >> p.defense >> p.health;

        group.push_back(p);
    }

    select(k, group, [](const pokemon& l, const pokemon& r) {
        return l.attack > r.attack;
    }, chosen);

    select(k, group, [](const pokemon& l, const pokemon& r) {
        return l.defense > r.defense;
    }, chosen);

    select(k, group, [](const pokemon& l, const pokemon& r) {
        return l.health > r.health;
    }, chosen);

    std::cout << chosen.size() << std::endl;

    return 0;
}