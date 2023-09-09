#include <iostream>
#include <deque>
#include <string>


class teque {
private:
    std::deque<int> m_front;
    std::deque<int> m_back;

private:
    void rebalance() {
        const int diff = (int) m_front.size() - (int) m_back.size();
        if (diff > 1) {
            int value_to_move = m_front.back();
            m_front.pop_back();
            m_back.push_front(value_to_move);
        }
        else if (diff < 0) {
            int value_to_move = m_back.front();
            m_back.pop_front();
            m_front.push_back(value_to_move);
        }
    }

public:
    void push_back(int m_value) {
        m_back.push_back(m_value);
        rebalance();
    }


    void push_front(int m_value) {
        m_front.push_front(m_value);
        rebalance();
    }


    void push_middle(int m_value) {
        const int index = (m_front.size() + m_back.size() + 1) / 2;
        if (index < m_front.size()) {
            m_front.push_back(m_value);
        }
        else {
            m_back.push_front(m_value);
        }

        rebalance();
    }


    int get(int index) {
        if (index < m_front.size()) {
            return m_front[index];
        }

        return m_back[index - m_front.size()];
    }
};


int main() {
    int n = 0;
    std::cin >> n;

    teque collection;

    for (int i = 0; i < n; i++) {
        std::string command;
        int argument;

        std::cin >> command >> argument;

        if (command == "push_back") {
            collection.push_back(argument);
        }
        else if (command == "push_front") {
            collection.push_front(argument);
        }
        else if (command == "push_middle") {
            collection.push_middle(argument);
        }
        else {
            std::cout << collection.get(argument) << std::endl;
        }
    }

    return 0;
}