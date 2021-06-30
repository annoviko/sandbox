#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>


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


class knuth_algorithm {
private:
    long long m_border = 0;
    std::size_t m_pattern_length = 0;
    std::string m_text;
    std::vector<long long> m_match;

public:
    knuth_algorithm(const std::string & p_pattern, const std::string & p_text) :
        m_pattern_length(p_pattern.size()),
        m_text(p_pattern + "$" + p_text),
        m_match(m_text.size(), 0)
    { }

public:
    std::vector<std::size_t> occurrences() {
        build_match();

        std::vector<std::size_t> result = { };
        for (std::size_t i = m_pattern_length; i < m_match.size(); i++) {
            if (m_match[i] == m_pattern_length) {
                result.push_back(i - 2 * m_pattern_length);
            }
        }

        return result;
    }

private:
    void build_match() {
        for (std::size_t i = 1; i < m_text.size(); i++) {
            while ((m_border > 0) && (m_text[i] != m_text[m_border])) {
                m_border = m_match[m_border - 1];
            }

            if ((m_border >= 0) && (m_text[m_border] == m_text[i])) {
                m_match[i] = ++m_border;
            }
            else {
                m_match[i] = 0;
            }
        }
    }
};


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


struct match_result {
public:
    std::size_t m_left  = -1;
    std::size_t m_right = -1;
    bool is_match       = false;

public:
    match_result() = default;
};


match_result find_match(const std::vector<std::size_t> & p_suffix, const std::string & p_text, const std::string & p_pattern) {
    match_result result;

    std::size_t min_index = 0;
    std::size_t max_index = p_text.size();

    while (min_index < max_index) {
        std::size_t middle = (min_index + max_index) / 2;
        std::string suffix = p_text.substr(p_suffix[middle], p_pattern.size());

        if (p_pattern > suffix) {
            min_index = middle + 1;
        }
        else {
            max_index = middle;
        }
    }

    std::size_t start = min_index;

    max_index = p_text.size();
    while (min_index < max_index) {
        std::size_t middle = (max_index + min_index) / 2;
        std::string suffix = p_text.substr(p_suffix[middle], p_pattern.size());

        if (p_pattern < suffix) {
            max_index = middle;
        }
        else {
            min_index = middle + 1;
        }
    }

    std::size_t end = max_index;

    if (start >= end) {
        match_result();
    }

    result.m_left = start;
    result.m_right = end;
    result.is_match = true;

    return result;
}


std::ostream & operator << (std::ostream & p_stream, std::vector<std::size_t> & p_vec) {
    p_stream << "[ ";
    for (auto val : p_vec) {
        p_stream << val << " ";
    }
    p_stream << std::endl;
    return p_stream;
}


std::ostream & operator << (std::ostream & p_stream, std::set<std::size_t> & p_vec) {
    p_stream << "[ ";
    for (auto val : p_vec) {
        p_stream << val << " ";
    }
    p_stream << std::endl;
    return p_stream;
}


void test() {
    while (true) {
        std::size_t length = std::rand() % 10000 + 1;
        std::string text;
        for (std::size_t i = 0; i < length; i++) {
            int value = std::rand() % ('z' - 'a') + 'a';
            text.push_back(value);
        }
        text += "$";

        length = std::rand() % 20 + 1;
        std::string pattern;
        for (std::size_t i = 0; i < length; i++) {
            char value = std::rand() % ('z' - 'a') + 'a';
            pattern.push_back(value);
        }

        auto result1_vector = knuth_algorithm(pattern, text).occurrences();
        std::set<std::size_t> result1(result1_vector.begin(), result1_vector.end());

        std::vector<std::size_t> suffix = suffix_builder(text).get_array();
        auto result2 = find_match(suffix, text, pattern);

        for (std::size_t i = result2.m_left; i < result2.m_right; i++) {
            auto iter = result1.find(suffix[i]);
            if (iter == result1.end()) {
                std::cout << "FAILURE" << std::endl;
                std::cout << "Text: " << text << std::endl;
                std::cout << "Pattern: " << pattern << std::endl;
                std::cout << "Following position is incorrect: " << i << ", suffix: " << text.substr(i) << std::endl;
                std::exit(-1);
            }

            result1.erase(iter);
        }

        if (!result1.empty()) {
            std::cout << "FAILURE" << std::endl;
            std::cout << "Text: " << text << std::endl;
            std::cout << "Pattern: " << pattern << std::endl;
            std::cout << "There are missed positions: " << result1 << std::endl;
            std::exit(-2);
        }

        std::cout << "SUCCESS" << std::endl;
    }
}


int main() {
    test();

    std::string text;
    std::getline(std::cin, text);
    text += "$";

    std::size_t n;
    std::cin >> n;

    std::vector<std::string> patterns(n);
    for (auto & pattern : patterns) {
        std::cin >> pattern;
    }

    std::vector<std::size_t> suffix = suffix_builder(text).get_array();
    for (auto & pattern : patterns) {
        auto result = find_match(suffix, text, pattern);

        if (!result.is_match) { continue; }

        for (std::size_t i = result.m_left; i < result.m_right; i++) {
            std::cout << suffix[i] << " ";
        }
    }

    return 0;
}