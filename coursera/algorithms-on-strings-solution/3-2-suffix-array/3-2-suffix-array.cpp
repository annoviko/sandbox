#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <list>


std::vector<std::size_t> get_suffix_naive(const std::string & p_text) {
    std::vector<std::size_t> result;

    std::map<std::string, std::size_t> suffix;

    for (std::size_t i = 0; i < p_text.size(); i++) {
        suffix.insert({ p_text.substr(i), i });
    }

    for (const auto & pair : suffix) {
        result.push_back(pair.second);
    }

    return result;
}



class suffix_builder {
private:
    std::string m_text;

    std::vector<std::size_t> m_order;
    std::vector<std::size_t> m_class;

public:
    suffix_builder(const std::string & p_text) :
        m_text(p_text),
        m_order(p_text.size(), 0),
        m_class(p_text.size(), 0)
    { }

public:
    std::vector<std::size_t> get_array() {
        m_order = sort_char();
        m_class = compute_char_classes();

        std::size_t length = 1;
        for (; length < m_text.size(); length *= 2) {
            m_order = sort_doubled(length);
            m_class = compute_classes(length);
        }

        return m_order;
    }

private:
    std::vector<std::size_t> sort_char() const {
        std::vector<std::size_t> count(127, 0);
        std::vector<std::size_t> order(m_text.size());

        for (auto symbol : m_text) {
            count[symbol]++;
        }

        for (std::size_t i = 1; i < count.size(); i++) {
            count[i] += count[i - 1];
        }

        for (std::size_t i = m_text.size(); i > 0; i--) {
            const char symbol = m_text[i - 1];
            count[symbol] -= 1;

            const std::size_t index = count[symbol];
            order[index] = i - 1;
        }

        return order;
    }

    std::vector<std::size_t> compute_char_classes() const {
        std::vector<std::size_t> new_class(m_text.size(), 0);
        for (std::size_t i = 1; i < m_text.size(); i++) {
            auto c1 = m_text[m_order[i]];
            auto c2 = m_text[m_order[i - 1]];
            if (m_text[m_order[i]] != m_text[m_order[i - 1]]) {
                new_class[m_order[i]] = new_class[m_order[i - 1]] + 1;
            }
            else {
                new_class[m_order[i]] = new_class[m_order[i - 1]];
            }
        }

        return new_class;
    }

    std::vector<std::size_t> sort_doubled(const std::size_t p_len) const {
        std::vector<std::size_t> count(m_text.size(), 0);
        std::vector<std::size_t> new_order(m_text.size(), 0);

        for (std::size_t i = 0; i < m_text.size(); i++) {
            count[m_class[i]] += 1;
        }

        for (std::size_t i = 1; i < count.size(); i++) {
            count[i] += count[i - 1];
        }

        for (std::size_t i = m_text.size(); i > 0; i--) {
            std::size_t position = (m_order[i - 1] + m_text.size() - p_len) % m_text.size();
            std::size_t index_class = m_class[position];
            count[index_class]--;

            std::size_t index_order = count[index_class];
            new_order[index_order] = position;
        }

        return new_order;
    }

    std::vector<std::size_t> compute_classes(const std::size_t p_len) const {
        std::size_t n = m_order.size();
        std::vector<std::size_t> new_class(n, 0);

        for (std::size_t i = 1; i < m_order.size(); i++) {
            std::size_t cur = m_order[i];
            std::size_t prev = m_order[i - 1];

            std::size_t mid = (cur + p_len) % n;
            std::size_t mid_prev = (prev + p_len) % n;

            if ((m_class[cur] != m_class[prev]) || (m_class[mid] != m_class[mid_prev])) {
                new_class[cur] = new_class[prev] + 1;
            }
            else {
                new_class[cur] = new_class[prev];
            }
        }

        return new_class;
    }
};



std::vector<std::size_t> get_suffix(const std::string & p_text) {
    std::vector<std::size_t> result;
    return result;
}


std::ostream & operator<<(std::ostream & p_stream, const std::vector<std::size_t> & p_result) {
    p_stream << "[ ";
    for (const auto & elem : p_result) {
        p_stream << elem << " ";
    }
    p_stream << "]";
    return p_stream;
}


void assert_eq(const std::vector<std::size_t> & p_expected, const std::vector<std::size_t> & p_actual) {
    if (p_expected != p_actual) {
        std::cout << "FAILURE: " << p_expected << " != " << p_actual << std::endl;
        std::exit(-1);
    }
    else {
        std::cout << "SUCCESS" << std::endl;
    }
}


void test() {
    assert_eq(get_suffix_naive("AAA$"), suffix_builder("AAA$").get_array());
    assert_eq(get_suffix_naive("GAC$"), suffix_builder("GAC$").get_array());
    assert_eq(get_suffix_naive("GAGAGAGA$"), suffix_builder("GAGAGAGA$").get_array());
    assert_eq(get_suffix_naive("AACGATAGCGGTAGA$"), suffix_builder("AACGATAGCGGTAGA$").get_array());
}


#if 0
    #define TEST
#endif


int main() {
#ifdef TEST
    test();
#else
    std::string text;
    std::getline(std::cin, text);

    auto suffix = suffix_builder(text).get_array();
    for (auto & value : suffix) {
        std::cout << value << " ";
    }
#endif
    return 0;
}