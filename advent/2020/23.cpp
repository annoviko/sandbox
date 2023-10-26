#include <algorithm>
#include <iostream>
#include <fstream>
#include <memory>
#include <set>
#include <string>


struct item {
public:
    using ptr = std::shared_ptr<item>;

public:
    long long m_value = 0;

    item::ptr m_next = nullptr;
    item::ptr m_prev = nullptr;

public:
    static item::ptr make_ptr(const long long p_value) {
        return std::make_shared<item>(p_value);
    }

public:
    item(const long long p_value) : m_value(p_value) { }

public:
    char get_value_as_symbol() const {
        return static_cast<char>(m_value + 48);
    }

    long long get_value() const {
        return m_value;
    }
};


class list {
private:
    constexpr static long long DEFAULT_PART_ONE_LIMIT = 10;
    constexpr static long long DEFAULT_PART_SECOND_LIMIT = 1000001;

public:
    enum class version { one, two };

private:
    item::ptr * m_map;

    item::ptr m_begin = nullptr;

    item::ptr m_cur = nullptr;    /* current cup */

    long long m_max = std::numeric_limits<long long>::min();
    long long m_min = std::numeric_limits<long long>::max();

public:
    list(const std::string & p_value, const version p_version) {
        const long long limit = (p_version == version::one) ? DEFAULT_PART_ONE_LIMIT : DEFAULT_PART_SECOND_LIMIT;
        m_map = new item::ptr[limit];

        m_begin = item::make_ptr(p_value[0] - 48);
        m_cur = m_begin;
        update_min_max(m_begin);

        item::ptr prev = m_begin;
        item::ptr cur = nullptr;
        m_map[m_begin->m_value] = m_begin;

        for (std::size_t i = 1; i < p_value.size(); i++) {
            cur = item::make_ptr(p_value[i] - 48);
            update_min_max(cur);

            m_map[cur->m_value] = cur;

            prev->m_next = cur;
            cur->m_prev = prev;

            prev = cur;
        }

        if (p_version == version::two) {
            for (long long i = m_max + 1; i < DEFAULT_PART_SECOND_LIMIT; i++) {
                cur = item::make_ptr(i);

                m_map[cur->m_value] = cur;

                prev->m_next = cur;
                cur->m_prev = prev;

                prev = cur;
            }

            m_max = DEFAULT_PART_SECOND_LIMIT - 1;
        }

        cur->m_next = m_begin;
        m_begin->m_prev = cur;

        m_cur = m_begin;
    }

    ~list() {
        delete[] m_map;
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

    std::string get_content_after(const long long p_value) {
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

    std::size_t get_multiplication_after(const long long p_value) {
        item::ptr begin = m_begin;
        while (begin->get_value() != 1) {
            begin = begin->m_next;
        }

        return begin->m_next->get_value() * begin->m_next->m_next->get_value();
    }

private:
    void update_min_max(const item::ptr & p_item) {
        m_min = std::min(m_min, p_item->get_value());
        m_max = std::max(m_max, p_item->get_value());
    }

    inline item::ptr get_left_item() const {
        return m_cur->m_next;
    }

    inline item::ptr get_right_item() const {
        return m_cur->m_next->m_next->m_next;
    }

    long long get_destination_cup(const item::ptr & p_lborder, const item::ptr & p_rborder) const {
        std::set<long long> unavailable_values;
        for (item::ptr cur = p_lborder; cur != p_rborder->m_next; cur = cur->m_next) {
            unavailable_values.insert(cur->get_value());
        }

        long long destination = m_cur->get_value() - 1;
        for (; destination >= m_min; --destination) {
            if (unavailable_values.find(destination) == unavailable_values.end()) {
                break;
            }
        }

        if (destination < m_min) {
            destination = m_max;
            while (unavailable_values.find(destination) != unavailable_values.end()) {
                --destination;
            }
        }

        return destination;
    }

    void move() {
        item::ptr lborder = get_left_item();
        item::ptr rborder = get_right_item();

        /* 1. remove range from the list */
        m_cur->m_next = rborder->m_next;
        rborder->m_next->m_prev = m_cur;

        /* 2. destination cup */
        long long destination_cup = get_destination_cup(lborder, rborder);
        item::ptr ldestination = m_map[destination_cup];
        item::ptr rdestination = ldestination->m_next;

        /* 3. place range after destination cup */
        ldestination->m_next = lborder;
        lborder->m_prev = ldestination;

        rborder->m_next = rdestination;
        rdestination->m_prev = rborder;

        /* 4. get next current cup */
        m_cur = m_cur->m_next;
    }
};


int main()
{
    const std::string input_value = "643719258";

    list container_first_version(input_value, list::version::one);

    container_first_version.process(100);
    std::cout << "Labels after (1): " << container_first_version.get_content_after(1) << std::endl;

    list container_second_version(input_value, list::version::two);
    container_second_version.process(10000000);
    std::cout << "Multiplication after (1): " << container_second_version.get_multiplication_after(1) << std::endl;

    return 0;
}
