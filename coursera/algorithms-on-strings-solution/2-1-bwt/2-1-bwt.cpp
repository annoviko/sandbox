#include <algorithm>
#include <iostream>
#include <string>
#include <vector>


using matrix = std::vector<std::string>;


matrix build_bwt_matrix(const std::string & p_text) {
	matrix result(p_text.size(), std::string());
	result[0] = p_text;

	for (std::size_t row = 1; row < p_text.size(); row++) {
		result[row] = result[row - 1].substr(p_text.size() - 1, 1) + result[row - 1].substr(0, p_text.size() - 1);
	}

	std::sort(result.begin(), result.end());

	return result;
}


std::string get_bwt(const matrix & p_bwt_matrix) {
	std::string result;
	result.reserve(p_bwt_matrix.size());

	for (auto & row : p_bwt_matrix) {
		result.push_back(row.back());
	}

	return result;
}



int main() {
	std::string text;
	std::getline(std::cin, text);

	matrix bwt_matrix = build_bwt_matrix(text);
	std::cout << get_bwt(bwt_matrix) << std::endl;

	return 0;
}
