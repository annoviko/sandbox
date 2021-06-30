#include <algorithm>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <unordered_map>


using matrix = std::vector<std::string>;


class text_restorer {
private:
	std::string m_btw;
	matrix m_matrix;
	std::size_t m_count = 0;

public:
	text_restorer(const std::string & p_btw) : 
		m_btw(p_btw),
		m_matrix(p_btw.size(), std::string(p_btw.size(), ' '))
	{ }

public:
	std::string restore() {
		std::string first = m_btw;
		std::sort(first.begin(), first.end());

		m_count = 1;
		fill_column(0, first);

		while (m_count != m_btw.size()) {
			insert_btw();
			std::sort(m_matrix.begin(), m_matrix.end());
			m_count++;
		}

		for (auto & text : m_matrix) {
			if (text.back() == '$') {
				return text;
			}
		}
	}

private:
	void insert_btw() {
		for (std::size_t i = 0; i < m_matrix.size(); i++) {
			const std::string row = m_btw[i] + m_matrix[i].substr(0, m_count);
			m_matrix[i] = std::move(row);
		}
	}

	void fill_column(const std::size_t p_index, const std::string & p_line) {
		for (std::size_t i = 0; i < m_matrix.size(); i++) {
			m_matrix[i][p_index] = p_line[i];
		}
	}
};


class text_inverse_restorer {
private:
	std::string m_first;
	std::string m_last;

	std::unordered_map<std::string, std::size_t> m_first_to_last;
	std::vector<std::string> m_last_to_first;

public:
	text_inverse_restorer(const std::string & p_btw) :
		m_last_to_first(p_btw.size())
	{
		m_last = p_btw;
		m_first = p_btw;
		std::sort(m_first.begin(), m_first.end());

		char symbol = -1;
		std::size_t number = 0;

		/*
		
		$0->0     A0->0
		A0->1     C0->1
		A1->2     $0->2
		C0->3     A1->3
		
		*/

		std::unordered_map<char, std::size_t> stats;

		for (std::size_t i = 0; i < m_first.size(); i++) {
			stats[m_first[i]]++;

			if (m_first[i] != symbol) {
				symbol = m_first[i];
				number = 0;
			}
			else {
				number++;
			}

			std::string key = symbol + std::to_string(number);
			m_first_to_last[key] = i;
		}

		for (std::size_t i = p_btw.size(); i > 0; i--) {
			symbol = p_btw[i - 1];
			number = --stats[symbol];

			std::string key = symbol + std::to_string(number);
			m_last_to_first[i - 1] = key;
		}
	}

public:
	std::string restore() {
		std::string result(m_first_to_last.size(), '$');

		std::string key = "$1";

		for (auto iter = result.rbegin() + 1; iter != result.rend(); iter++) {
			std::size_t index_line = m_first_to_last[key];

			*iter = m_last[index_line];

			key = m_last_to_first[index_line];
		}

		return result;
	}
};

int main() {
	std::string btw;
	std::getline(std::cin, btw);

	//std::cout << text_restorer(btw).restore() << std::endl;
	std::cout << text_inverse_restorer(btw).restore() << std::endl;

	return 0;
}
