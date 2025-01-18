#include <iostream>
#include <vector>
#include <algorithm>


struct consumer {
    std::int64_t amount = 0;
    std::int64_t price = 0;
};


int main() {
    std::int64_t n, m;
    std::cin >> n >> m;

    std::vector<std::int64_t> fish(n);
    for (int i = 0; i < n; i++) {
        std::cin >> fish[i];
    }

    std::vector<consumer> clients(m);
    for (int i = 0; i < m; i++) {
        std::cin >> clients[i].amount >> clients[i].price;
    }

    std::sort(fish.begin(), fish.end(), std::greater<>());
    std::sort(clients.begin(), clients.end(), [](const consumer& l, const consumer& r) {
        return l.price > r.price;
    });

    std::int64_t money = 0;
    for (int i = 0, j = 0; i < fish.size() && j < clients.size(); j++) {
        int transaction_amount = std::min((std::int64_t) fish.size() - i, clients[j].amount);
        for (int k = 0; k < transaction_amount; k++) {
            money += fish[i] * clients[j].price;
            i++;
        }
    }

    std::cout << money << std::endl;

    return 0;
}