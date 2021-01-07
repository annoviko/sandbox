#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <vector>


using deck_sequence = std::list<std::size_t>;


class player {
private:
    deck_sequence   m_seq;

public:
    player() = default;

    player(const deck_sequence & p_seq) :
        m_seq(p_seq)
    { }

public:
    bool operator==(const player & p_other) const {
        return m_seq == p_other.m_seq;
    }

public:
    void add(const std::size_t p_card) {
        m_seq.push_back(p_card);
    }

    void add(const std::size_t p_card1, const std::size_t p_card2) {
        add(p_card1);
        add(p_card2);
    }

    std::size_t get() {
        std::size_t card = m_seq.front();
        m_seq.pop_front();
        return card;
    }

    deck_sequence copy(const std::size_t p_len) {
        deck_sequence result;

        std::size_t index = 0;
        for (auto iter = m_seq.begin(); index < p_len; iter++, index++) {
            result.push_back(*iter);
        }

        return result;
    }

    bool empty() const {
        return m_seq.empty();
    }

    std::size_t size() {
        return m_seq.size();
    }

    std::size_t score() {
        std::size_t result = 0;

        std::size_t multiplier = m_seq.size();
        for (auto iter = m_seq.begin(); iter != m_seq.end(); iter++) {
            result += *iter * multiplier;
            multiplier--;
        }

        return result;
    }
};


class combat {
protected:
    player m_first;
    player m_second;

public:
    combat(const player & p_player1, const player & p_player2) :
        m_first(p_player1), m_second(p_player2)
    { }

    std::size_t run() {
        while (!m_first.empty() && !m_second.empty()) {
            make_step();
        }

        return m_first.empty() ? m_second.score() : m_first.score();
    }

protected:
    void make_step() {
        const std::size_t card1 = m_first.get();
        const std::size_t card2 = m_second.get();

        if (card1 > card2) {
            m_first.add(card1, card2);
        }
        else if (card1 < card2) {
            m_second.add(card2, card1);
        }
        else {
            throw std::exception("There is no rules to handle it.");
        }
    }
};


enum class reading_state {
    NAME, CARD
};


std::vector<player> read(const std::string & p_file) {
    std::fstream stream(p_file);

    std::vector<player> result;

    reading_state state = reading_state::NAME;
    player current;

    for (std::string line; std::getline(stream, line);) {
        switch (state)
        {
        case reading_state::NAME:
            state = reading_state::CARD;
            break;
        case reading_state::CARD:
            if (line.empty()) {
                state = reading_state::NAME;
                result.push_back(current);
                current = player();
            }
            else {
                current.add(std::stoull(line));
            }
            break;
        default:
            break;
        }
    }

    result.push_back(current);
    return result;
}


class combat_recursive {
private:
    enum class status { CONTINUE, INTERRUPT };

private:
    player m_first, m_second;

    std::size_t m_winner = -1;

    bool m_combinations[10000];

    std::size_t m_counter = 0;
    std::size_t m_threshold = -1;

public:
    combat_recursive(const player & p_player1, const player & p_player2) :
        m_first(p_player1),
        m_second(p_player2),
        m_threshold(m_first.size() + m_second.size())
    { }

    combat_recursive & run() {
        while (!m_first.empty() && !m_second.empty()) {
            if (make_step() == status::INTERRUPT) {
                return *this;
            }
        }

        m_winner = m_first.empty() ? 2 : 1;
        return *this;
    }

    std::size_t get_winner_index() {
        return m_winner;
    }

    player & get_winner() {
        switch (m_winner) {
            case 1: return m_first;
            case 2: return m_second;
            default: throw std::exception("Impossible to determine winner.");
        }
    }

private:
    status make_step() {
        const std::size_t card1 = m_first.get();
        const std::size_t card2 = m_second.get();

        const std::size_t combination = card1 * 100 + card2;
        if (m_combinations[combination]) {
            m_counter++;
            if (m_counter > m_threshold) {
                m_winner = 1;
                return status::INTERRUPT;
            }
        }
        else {
            m_counter = 0;
        }

        m_combinations[combination] = true;


        if ((card1 <= m_first.size()) && (card2 <= m_second.size())) {
            player player1(m_first.copy(card1));
            player player2(m_second.copy(card2));

            combat_recursive sub_game(player1, player2);
            sub_game.run();

            switch (sub_game.get_winner_index()) {
            case 1:
                m_first.add(card1, card2);
                break;

            case 2:
                m_second.add(card2, card1);
                break;
            }
        }
        else {
            if (card1 > card2) {
                m_first.add(card1, card2);
            }
            else if (card1 < card2) {
                m_second.add(card2, card1);
            }
            else {
                throw std::exception("There is no rules to handle it.");
            }
        }

        return status::CONTINUE;
    }
};


int main()
{
    auto players = read("input.txt");

    combat game1(players[0], players[1]);

    std::cout << "The winning player's score (combat): " << game1.run() << std::endl;

    combat_recursive game2(players[0], players[1]);

    std::cout << "The winning player's score (recursive combat): " << game2.run().get_winner().score() << std::endl;

    return 0;
}

