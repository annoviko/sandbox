#include <algorithm>
#include <iostream>
#include <cassert>
#include <string>
#include <set>
#include <vector>


long long eval(long long a, long long b, char op) {
	if (op == '*') {
		return a * b;
	}
	else if (op == '+') {
		return a + b;
	}
	else if (op == '-') {
		return a - b;
	}
	else {
		assert(0);
	}
}

using matrix = std::vector<std::vector<long long>>;

void fill_matrix_value(const std::size_t p_row, const std::size_t p_col, const std::size_t p_len, const std::string & exp, matrix & p_min, matrix & p_max) {
	long long min_value = std::numeric_limits<long long>::max();
	long long max_value = std::numeric_limits<long long>::min();

	// 1+2-5
	// (1, 1), (2, 3) rborder = 1
	// (1, 2), (3, 3) rborder = 2

	std::set<long long> result;
	for (std::size_t rborder = p_row; rborder < p_len; rborder++) {
		const char op = exp[rborder * 2 + 1];

		long long candidate_min_value = eval(p_min[p_row][rborder], p_min[rborder + 1][p_col], op);
		long long candidate_max_value = eval(p_max[p_row][rborder], p_max[rborder + 1][p_col], op);

		result.insert(candidate_min_value);
		result.insert(candidate_max_value);
	}

	p_min[p_row][p_col] = *std::min_element(result.begin(), result.end());
	p_max[p_row][p_col] = *std::max_element(result.begin(), result.end());
}

long long get_maximum_value(const std::string &exp) {
	const std::size_t mtrx_size = exp.size() / 2 + 1;

	matrix mtrx_min(mtrx_size, std::vector<long long>(mtrx_size, 0));
	matrix mtrx_max(mtrx_size, std::vector<long long>(mtrx_size, 0));

	for (std::size_t i = 0; i < mtrx_size; i++) {
		const long long value = std::stoll(exp.substr(i * 2, 1));
		mtrx_min[i][i] = value;
		mtrx_max[i][i] = value;
	}

	for (std::size_t offset = 1; offset < mtrx_size; offset++) {
		for (std::size_t row = 0; row + offset < mtrx_size; row++) {
			const std::size_t len = offset + row;
			fill_matrix_value(row, row + offset, len, exp, mtrx_min, mtrx_max);
		}
	}

	return mtrx_max.front().back();
}

int main() {
	std::string s;
	std::cin >> s;
	std::cout << get_maximum_value(s) << '\n';
}
