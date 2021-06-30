#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>
#include <unordered_map>


using matrix = std::vector<std::string>;


class bwt_matcher {
private:
	std::string m_first;
	std::string m_last;

	std::vector<std::size_t> m_stats = std::vector<std::size_t>(127);
	std::vector<std::vector<std::size_t>> m_count = std::vector<std::vector<std::size_t>>(127);
	std::vector<std::size_t> m_first_occurrence = std::vector<std::size_t>(127);

public:
	bwt_matcher(const std::string & p_btw) {
		m_last = p_btw;
		m_first = p_btw;
		std::sort(m_first.begin(), m_first.end());

		for (std::size_t i = 0; i < p_btw.size(); i++) {
			char symbol = p_btw[i];

			if (m_count[symbol].empty()) {
				m_count[symbol] = std::vector<std::size_t>(p_btw.size() + 1, 0);
			}

			auto & count_symbol = m_count[symbol];
			count_symbol[i] = m_stats[symbol]++;
			std::size_t occurrence = m_stats[symbol];
			for (std::size_t j = i + 1; j < count_symbol.size(); j++) {		// O(N^2)
				count_symbol[j] = occurrence;
			}

			symbol = m_first[i];
			if (m_first_occurrence[symbol] == 0) {
				m_first_occurrence[symbol] = i;
			}
		}
	}

public:
	std::size_t match(const std::string & p_pattern) {
		std::size_t result = 0;

		std::size_t top = 0;
		std::size_t bottom = m_last.size() - 1;

		for (auto iter = p_pattern.rbegin(); iter != p_pattern.rend(); iter++) {
			char symbol = *iter;
			if (m_stats[symbol] != 0) {
				top = m_first_occurrence[symbol] + m_count[symbol][top];
				bottom = m_first_occurrence[symbol] + m_count[symbol][bottom + 1] - 1;
				//std::cout << top << ", " << bottom << std::endl;
			}
			else {
				return 0;
			}
		}

		return bottom - top + 1;
	}
};

#if 1
class bwt_matcher_fast {
private:
	std::string m_first;
	std::string m_last;

	std::vector<std::size_t> m_stats = std::vector<std::size_t>(127);
	std::vector<std::map<std::size_t, std::size_t>> m_count;
	std::vector<std::size_t> m_first_occurrence = std::vector<std::size_t>(127);

public:
	bwt_matcher_fast(const std::string & p_btw) : m_count(127) {
		m_last = p_btw;
		m_first = p_btw;
		std::sort(m_first.begin(), m_first.end());	// N*log(N)

		for (std::size_t i = 0; i < p_btw.size(); i++) {	// N
			char symbol = p_btw[i];

			auto & count_symbol = m_count[symbol];	// O(1)
			if (count_symbol.empty()) {
				count_symbol[0] = 0;
			}
			count_symbol[i + 1] = ++m_stats[symbol];

			symbol = m_first[i];
			if (m_first_occurrence[symbol] == 0) {
				m_first_occurrence[symbol] = i;
			}
		}

		for (std::size_t i = 0; i < p_btw.size(); i++) {	// N
			char symbol = p_btw[i];
			m_count[symbol][p_btw.size()] = m_stats[symbol];	// log(N) -> N * log(N)
		}
	}

public:
	std::size_t match(const std::string & p_pattern) {
		std::size_t result = 0;

		std::size_t top = 0;
		std::size_t bottom = m_last.size() - 1;

		for (auto iter = p_pattern.rbegin(); iter != p_pattern.rend(); iter++) {
			char symbol = *iter;
			if (m_stats[symbol] != 0) {
				top = m_first_occurrence[symbol] + get_count(symbol, top);
				bottom = m_first_occurrence[symbol] + get_count(symbol, bottom + 1) - 1;
				//std::cout << top << ", " << bottom << std::endl;
			}
			else {
				return 0;
			}
		}

		return bottom - top + 1;
	}

	std::size_t get_count(const char p_symbol, const std::size_t p_position) {
		auto iter = m_count[p_symbol].lower_bound(p_position);
		if (iter->first > p_position) {
			iter--;
		}

		return iter->second;
	}
};
#endif


int main() {
	std::string bwt;
	std::getline(std::cin, bwt);

	std::size_t n = 0;
	std::cin >> n;
	std::cin.ignore(1);

	std::vector<std::string> patterns(n);
	for (std::size_t i = 0; i < n; i++) {
		std::cin >> patterns[i];
	}

	bwt_matcher_fast matcher(bwt);
	for (const auto & pattern : patterns) {
		std::cout << matcher.match(pattern) << " ";
	}

	return 0;
}