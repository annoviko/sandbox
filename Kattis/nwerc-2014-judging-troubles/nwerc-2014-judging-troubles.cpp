#include <iostream>
#include <string>
#include <set>


int main() {
	std::size_t n;
	std::cin >> n;
	std::cin.ignore(1);

	std::multiset<std::string> dom_system;
	for (std::size_t i = 0; i < n; i++) {
		std::string verdict;
		std::getline(std::cin, verdict);

		dom_system.insert(verdict);
	}

	std::multiset<std::string> kattis_system;
	for (std::size_t i = 0; i < n; i++) {
		std::string verdict;
		std::getline(std::cin, verdict);

		kattis_system.insert(verdict);
	}

	std::size_t same_result = 0;
	for (const auto & verdict : kattis_system) {
		auto iter = dom_system.find(verdict);
		if (iter != dom_system.end()) {
			same_result++;
			dom_system.erase(iter);
		}
	}

	std::cout << same_result << std::endl;

	return 0;
}