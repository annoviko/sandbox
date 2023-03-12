#include <string>
#include <set>
#include <vector>
#include <unordered_set>
#include <unordered_map>


class solution {
public:
    bool is_possible(const std::string& p_string) {
        std::vector<int> offsets = { 0, 1, 2, 3, 4, 5, 6 };

        for (int i = 0; i < offsets.size(); i++) {
            std::unordered_set<char> initial_group;
            int unique_values = offsets[i];
            for (int j = 0; (j < unique_values) && (j < p_string.size()); j++) {
                initial_group.insert(p_string[j]);
            }

            if (initial_group.size() != unique_values) {
                continue;
            }

            int counter = 0;
            bool checked = offsets[i] < p_string.size();
            bool status = true;
            std::unordered_set<char> group;

            for (int j = offsets[i]; j < p_string.size(); j++) {
                const char value = p_string[j];

                if (counter >= 7) {
                    counter = 0;
                    group.clear();
                }

                counter++;

                group.insert(value);
                if (group.size() != counter) {
                    status = false;
                    break;
                }
            }

            if (checked && status) {
                return true;
            }
        }

        return false;
    }
};


#include <iostream>


void test() {
    std::unordered_map<std::string, bool> tests = {
        { "JJTO", true },
        { "JJTT", false },
        { "JLSZIOT", true },
        { "JLSZIOTJLSZIOT", true },
        { "JLSZIOTJLSZITT", false },
        { "JLSZIOTTOIZSLJ", true },
        { "JLSZIOTTOIZSLJJ", true },
        { "JLSZIOTTOIZSLJJL", true },
        { "JLSZIOTTOIZSLJJJ", false },
        { "JLSZIOTTOIZSLJOO", false },
        { "LSZIOTJLSZIOTJLSZIOT", true },
        { "SZIOTJLSZIOTJLSZIOT", true },
        { "JOTO", true },
    };

    for (auto& pair : tests) {
        bool actual = solution().is_possible(pair.first);
        if (actual != pair.second) {
            std::cout << "[FAIL] string = " << pair.first << std::endl;
        }
        else {
            std::cout << "[PASS]" << std::endl;
        }
    }
}


int main() {
#if 0
    test();
#else
    int n = 0;
    std::cin >> n;

    for (int i = 0; i < n; i++) {
        std::string tetris_figures;
        std::cin >> tetris_figures;
        
        bool result = solution().is_possible(tetris_figures);
        std::cout << (result ? 1 : 0) << std::endl;
    }
#endif
    return 0;
}
