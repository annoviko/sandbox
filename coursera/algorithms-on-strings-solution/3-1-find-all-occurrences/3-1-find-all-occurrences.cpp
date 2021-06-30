#include <vector>
#include <iostream>
#include <string>


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


int main() {
    std::string pattern, text;
    std::getline(std::cin, pattern);
    std::getline(std::cin, text);

    auto result = knuth_algorithm(pattern, text).occurrences();

    for (const auto position : result) {
        std::cout << position << " ";
    }
    
    return 0;
}