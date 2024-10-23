#include <queue>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>


struct output {
    int sell = -1;
    int buy = -1;
    int last_deal = -1;
};


struct content {
    int amount;
    int price;
};


struct comparator_price_asc {
    bool operator()(const content& l, const content& r) {
        return l.price > r.price;
    }
};


struct comparator_price_desc {
    bool operator()(const content& l, const content& r) {
        return l.price < r.price;
    }
};


class stock {
    std::priority_queue<content, std::vector<content>, comparator_price_desc> buy_queue;
    std::priority_queue<content, std::vector<content>, comparator_price_asc> sell_queue;

    int last_deal = -1;

public:
    output sell(int amount, int price) {
        output result;

        while (!buy_queue.empty() && (buy_queue.top().price >= price) && (amount > 0)) {
            last_deal = price;   /*  shares are exchanged at the rate of the ask price (sell) */
            int transation_amount = std::min(buy_queue.top().amount, amount);
            
            ((content&) buy_queue.top()).amount -= transation_amount;
            amount -= transation_amount;

            if (buy_queue.top().amount == 0) {
                buy_queue.pop();
            }
        }

        if (amount > 0) {
            sell_queue.push({ amount, price });
        }

        result.buy = buy_queue.empty() ? -1 : buy_queue.top().price;
        result.sell = sell_queue.empty() ? -1 : sell_queue.top().price;
        result.last_deal = last_deal;

        return result;
    }

    output buy(int amount, int price) {
        output result;

        while (!sell_queue.empty() && (price >= sell_queue.top().price) && (amount > 0)) {
            last_deal = sell_queue.top().price;   /*  shares are exchanged at the rate of the ask price (sell) */
            int transaction_amount = std::min(sell_queue.top().amount, amount);

            ((content&)sell_queue.top()).amount -= transaction_amount;
            amount -= transaction_amount;

            if (sell_queue.top().amount == 0) {
                sell_queue.pop();
            }
        }

        if (amount > 0) {
            buy_queue.push({ amount, price });
        }

        result.buy = buy_queue.empty() ? -1 : buy_queue.top().price;
        result.sell = sell_queue.empty() ? -1 : sell_queue.top().price;
        result.last_deal = last_deal;

        return result;
    }
};


int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        int actions = 0;
        std::cin >> actions;

        stock solution;
        for (int j = 0; j < actions; j++) {
            std::string ignore, action;
            int amount, price;

            std::cin >> action >> amount >> ignore >> ignore >> price;

            output status;
            if (action == "sell") {
                status = solution.sell(amount, price);
            }
            else {
                status = solution.buy(amount, price);
            }

            std::cout << (status.sell >= 0 ? std::to_string(status.sell) : "-") << " " <<
                (status.buy >= 0 ? std::to_string(status.buy) : "-") << " " <<
                (status.last_deal >= 0 ? std::to_string(status.last_deal) : "-") << std::endl;
        }
    }

    return 0;
}