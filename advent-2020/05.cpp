#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <string>


class ticket {
private:
    std::string m_ticket;

    std::size_t m_row = 0;
    std::size_t m_col = 0;
    std::size_t m_id = 0;

public:
    ticket(const std::string & p_ticket) : m_ticket(p_ticket) {
        decode();
    }

public:
    std::size_t get_row() const { return m_row; }
    std::size_t get_col() const { return m_col; }
    std::size_t get_id() const { return m_id;  }

private:
    void decode() {
        std::size_t lborder = 0;
        std::size_t rborder = 127;

        for (std::size_t i = 0; i < 7; i++) {
            const char location = m_ticket[i];

            std::size_t middle = lborder + (rborder - lborder) / 2;

            switch (location) {
            case 'F':
                rborder = middle;
                break;
            case 'B':
                lborder = middle + 1;
                break;
            default:
                throw std::invalid_argument("Unexpected row signature.");
            }
        }

        m_row = rborder;

        lborder = 0;
        rborder = 7;

        for (std::size_t i = 7; i < m_ticket.size(); i++) {
            const char location = m_ticket[i];

            std::size_t middle = lborder + (rborder - lborder) / 2;

            switch (location) {
            case 'L':
                rborder = middle;
                break;
            case 'R':
                lborder = middle + 1;
                break;
            default:
                throw std::invalid_argument("Unexpected column signature.");
            }
        }

        m_col = rborder;
        m_id = m_row * 8 + m_col;
    }
};


std::list<ticket> read_tickets() {  /* O(n) */
    std::list<ticket> tickets;

    std::ifstream stream("input.txt");
    for (std::string line; std::getline(stream, line); ) {
        tickets.emplace_back(line);
    }

    return tickets;
}


std::size_t find_maximum_ticket_id(const std::list<ticket> & p_tickets) {   /* O(n) */
    return std::max_element(p_tickets.begin(), p_tickets.end(), [](const ticket & p_first, const ticket & p_second) {
            return p_first.get_id() < p_second.get_id();
        })->get_id();
}


std::size_t find_own_ticket_id(std::list<ticket> & p_tickets) { /* O(n * log(n)) */
    p_tickets.sort([](const ticket & p_first, const ticket & p_second) { return p_first.get_id() < p_second.get_id(); });

    auto prev = p_tickets.begin();
    auto cur = ++(p_tickets.begin());

    while (cur != p_tickets.end() && prev->get_id() + 1 != cur->get_id() - 1) {
        prev++;
        cur++;
    }

    if (cur == p_tickets.end()) {
        throw std::invalid_argument("Impossible to find your seat.");
    }

    return prev->get_id() + 1;
}


int main() {
    auto tickets = read_tickets();

    std::cout << "The highest seat ID: " << find_maximum_ticket_id(tickets) << std::endl;
    std::cout << "Your seat ID: " << find_own_ticket_id(tickets) << std::endl;

    return 0;
}
