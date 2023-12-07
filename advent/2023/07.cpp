#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>


enum hand_type {
    UNKNOWN = 0,
    HIGH_CARD = 1,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    FIVE_OF_A_KIND
};


std::unordered_map<hand_type, hand_type> JOKER_UPDATE = {
    { HIGH_CARD, ONE_PAIR },                /* 1234J -> 12344 */
    { ONE_PAIR, THREE_OF_A_KIND },          /* 1123J -> 11123 */
    { TWO_PAIR, FULL_HOUSE },               /* 1122J -> 11222 */
    { THREE_OF_A_KIND, FOUR_OF_A_KIND },    /* 1112J -> 11112 */
    { FULL_HOUSE, FOUR_OF_A_KIND },         /* 111JJ -> 1111J */
    { FOUR_OF_A_KIND, FIVE_OF_A_KIND },     /* 1111J -> 11111 */
    { FIVE_OF_A_KIND, FIVE_OF_A_KIND }
};


struct hand {
    std::string cards;
    std::uint64_t bid;
    hand_type type = UNKNOWN;
};


class camel_cards {
public:
    std::vector<hand> hands;
    bool joker_enabled;

public:
    camel_cards(const std::vector<hand>& p_hands, const bool enable_joker) : 
        hands(p_hands), joker_enabled(enable_joker)
    { }

public:
    std::uint64_t find_total_winning() {
        for (auto& h : hands) {
            h.type = identify_hand_type(h.cards);
        }

        std::sort(hands.begin(), hands.end(), [this](hand& l, hand& r) {
            if (l.type < r.type) {
                return true;
            }
            else if (l.type > r.type) {
                return false;
            }

            for (int i = 0; i < l.cards.size(); i++) {
                auto lvalue = get_card_value(l.cards[i]);
                auto rvalue = get_card_value(r.cards[i]);

                if (lvalue == rvalue) {
                    continue;
                }
                return lvalue < rvalue;
            }

            return false;
        });

        std::uint64_t score = 0;
        for (int i = 0; i < hands.size(); i++) {
            score += (i + 1) * hands[i].bid;
        }

        return score;
    }

private:
    std::uint64_t get_card_value(const char c) {
        if (std::isdigit(c)) {
            return c - '0';
        }

        switch (c) {
        case 'A': return 14;
        case 'K': return 13;
        case 'Q': return 12;
        case 'J': return joker_enabled ? 0 : 11;
        case 'T': return 10;
        }

        throw std::runtime_error("unexpected card");
    }

    hand_type identify_hand_type(const std::string& hand) {
        std::unordered_map<char, int> freq;

        for (auto c : hand) {
            freq[c]++;
        }

        std::vector<int> stats;
        std::uint64_t jokers = 0;
        for (auto& p : freq) {
            if (joker_enabled && p.first == 'J') {
                for (int i = 0; i < p.second; i++) {
                    stats.push_back(1);
                }

                jokers = p.second;
                continue;
            }
            
            stats.push_back(p.second);
        }

        std::sort(stats.begin(), stats.end());

        hand_type type = UNKNOWN;
        if (stats.size() == 5) {
            type = HIGH_CARD;
        }
        else if (stats.size() == 4) {
            type = ONE_PAIR;
        }
        else if (stats.size() == 3) {
            if (stats.back() == 3) {
                type = THREE_OF_A_KIND; /* TTT98 */
            }
            else {
                type = TWO_PAIR;    /* 23432 */
            }
        }
        else if (stats.size() == 2) {
            if (stats.back() == 3) {
                type = FULL_HOUSE;  /* 23332 */
            }
            else {
                type = FOUR_OF_A_KIND; /* AA8AA */
            }
        }
        else {
            type = FIVE_OF_A_KIND;
        }

        for (int i = 0; i < jokers; i++) {
            auto iter = JOKER_UPDATE.find(type);
            type = iter->second;
        }

        return type;
    }
};


std::vector<hand> read_input() {
    std::vector<hand> input;

    std::fstream stream("input.txt");
    while (stream.good()) {
        std::string cards;
        std::uint64_t bid;

        stream >> cards >> bid;
        input.push_back({ cards, bid });
    }

    return input;
}


int main() {
    auto input = read_input();

    std::cout << "The total winnings: " << camel_cards(input, false).find_total_winning() << std::endl;
    std::cout << "The total winnings (joker rule): " << camel_cards(input, true).find_total_winning() << std::endl;

    return 0;
}
