#include <algorithm>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>


struct item {
public:
    using ptr = std::shared_ptr<item>;

public:
    int m_value = 0;

    item::ptr m_next = nullptr;
    item::ptr m_prev = nullptr;

public:
    static item::ptr make_ptr(const char p_value) {
        return std::make_shared<item>(p_value);
    }

public:
    item(const char p_value) : m_value(p_value - 48) { }

public:
    char get_value_as_symbol() const {
        return static_cast<char>(m_value + 48);
    }

    int get_value() const {
        return m_value;
    }
};


class list {
private:
    item::ptr m_map[10];

    item::ptr m_begin = nullptr;

    item::ptr m_cur   = nullptr;    /* current cup */

    int m_max = INT_MIN;
    int m_min = INT_MAX;

    int m_length = 0;
    int m_current_index = 0;

public:
    list(const std::string & p_value) {
        m_length = p_value.size();
        m_current_index = 0;

        m_begin = item::make_ptr(p_value[0]);
        m_cur = m_begin;
        update_min_max(m_begin);

        item::ptr prev = m_begin;
        item::ptr cur  = nullptr;
        m_map[m_begin->m_value] = m_begin;

        for (std::size_t i = 1; i < p_value.size(); i++) {
            cur = item::make_ptr(p_value[i]);
            update_min_max(cur);

            m_map[cur->m_value] = cur;

            prev->m_next = cur;
            cur->m_prev = prev;

            prev = cur;
        }

        cur->m_next = m_begin;
        m_begin->m_prev = cur;

        m_cur = m_begin;
    }

public:
    void process(const std::size_t p_steps) {
        for (std::size_t i = 0; i < p_steps; i++) {
            move();
        }
    }

    std::string get_state() {
        std::string result;
        if (m_begin == m_cur) {
            result += "(" + std::to_string(m_begin->get_value()) + ")";
        }
        else {
            result.push_back(m_begin->get_value_as_symbol());
        }

        for (item::ptr cur = m_begin->m_next; cur != m_begin; cur = cur->m_next) {
            if (cur == m_cur) {
                result += "(" + std::to_string(cur->get_value()) + ")";
            }
            else {
                result.push_back(cur->get_value_as_symbol());
            }
        }

        return result;
    }

    std::string get_content() {
        std::string result(1, m_begin->get_value_as_symbol());
        for (item::ptr cur = m_begin->m_next; cur != m_begin; cur = cur->m_next) {
            result.push_back(cur->get_value_as_symbol());
        }

        return result;
    }

    std::string get_content_after(const int p_value) {
        std::string result;

        item::ptr begin = m_begin;
        while (begin->get_value() != 1) {
            begin = begin->m_next;
        }

        for (item::ptr cur = begin->m_next; cur != begin; cur = cur->m_next) {
            result.push_back(cur->get_value_as_symbol());
        }

        return result;
    }

private:
    void update_min_max(const item::ptr & p_item) {
        m_min = std::min(m_min, p_item->get_value());
        m_max = std::max(m_max, p_item->get_value());
    }

    item::ptr get_left_item() const {
        return m_cur->m_next;
    }

    item::ptr get_right_item() const {
        item::ptr result = m_cur;
        for (std::size_t i = 0; i < 3; i++) {
            result = result->m_next;
        }

        return result;
    }

    int get_destination_cup(const item::ptr & p_current, const item::ptr & p_lborder, const item::ptr & p_rborder) const {
        bool available_values[10];
        for (item::ptr cur = p_lborder; cur != p_rborder->m_next; cur = cur->m_next) {
            available_values[cur->get_value()] = false;
        }

        int destination = m_cur->get_value() - 1;
        for (; destination >= m_min; destination--) {
            if (available_values[destination]) {
                break;
            }
        }

        if (destination < m_min) {
            if (available_values[m_max]) {
                destination = m_max;
            }
            else {
                /* the maximum value in the range, we have to look for the maximum in the rest list */
                destination = p_current->get_value();
                for (item::ptr cur = p_current->m_next; cur != p_current; cur = cur->m_next) {
                    destination = std::max(destination, cur->get_value());
                }
            }
        }

        return destination;
    }

    void keep_order() {
        item::ptr next_begin = m_cur;
        for (int cursor = m_current_index; cursor > 0; cursor--) {
            next_begin = next_begin->m_prev;
        }

        m_begin = next_begin;
    }

    void move() {
        item::ptr lborder = get_left_item();
        item::ptr rborder = get_right_item();

        /* 1. remove range from the list */
        m_cur->m_next = rborder->m_next;
        rborder->m_next->m_prev = m_cur;

        /* 2. destination cup */
        int destination_cup = get_destination_cup(m_cur, lborder, rborder);
        item::ptr ldestination = m_map[destination_cup];
        item::ptr rdestination = ldestination->m_next;

        /* 3. place range after destination cup */
        ldestination->m_next = lborder;
        lborder->m_prev = ldestination;

        rborder->m_next = rdestination;
        rdestination->m_prev = rborder;

        /* 3.1. keep order */
        keep_order();

        /* 4. get next current cup */
        m_cur = m_cur->m_next;
        m_current_index++;

        if (m_current_index >= m_length) {
            m_current_index = 0;
        }
    }
};


int main()
{
    list container("643719258");

    container.process(100);
    std::cout << "Labels after (1): " << container.get_content_after(1) << std::endl;

    return 0;
}
