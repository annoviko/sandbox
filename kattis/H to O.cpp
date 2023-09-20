#include <climits>
#include <iostream>
#include <vector>
#include <string>


std::vector<std::int64_t> count(const std::string& p_formula, const std::int64_t p_multiplier) {
    std::vector<std::int64_t> counter('Z' - 'A' + 1, 0);

    for (int i = 0; i < p_formula.size();) {
        char elem = p_formula[i];

        std::string value;
        for (int j = i + 1; j < p_formula.size() && std::isdigit(p_formula[j]); j++) {
            value.push_back(p_formula[j]);
            i++;
        }

        std::int64_t n = value.empty() ? 1 : std::stoll(value);
        counter[elem - 'A'] += n * p_multiplier;
        i++;
    }

    return counter;
}


int main() {
    std::int64_t multiplier;
    std::string formula;

    std::cin >> formula >> multiplier;
    std::vector<std::int64_t> counter = count(formula, multiplier);

    std::cin >> formula;
    std::vector<std::int64_t> request = count(formula, 1);

    std::int64_t max_elems = LLONG_MAX;
    for (int i = 0; i < request.size(); i++) {
        if (request[i] > 0) {
            std::int64_t elems = counter[i] / request[i];
            max_elems = std::min(elems, max_elems);
        }
    }

    std::cout << max_elems << std::endl;
    return 0;
}