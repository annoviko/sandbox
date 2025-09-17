#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>


struct person {
    std::string name;
    std::string code;
};


int main() {
    int t = 0;
    std::cin >> t;

    for (int _t = 0; _t < t; _t++) {
        int n = 0;
        std::cin >> n;

        std::vector<person> codes;

        for (int i = 0; i < n; i++) {
            std::string name;
            std::cin >> name;
            name.pop_back();

            std::string person_class;
            std::cin >> person_class;

            std::stringstream ss(person_class);
            std::string class_code;
            std::string level;
            while (std::getline(ss, level, '-')) {
                if (level == "upper") {
                    class_code.push_back('2');
                }
                else if (level == "middle") {
                    class_code.push_back('1');
                }
                else {
                    class_code.push_back('0');
                }
            }

            std::reverse(class_code.begin(), class_code.end());

            while (class_code.size() < 10) class_code += '1';

            codes.push_back({ name, class_code });

            std::string ignore;
            std::cin >> ignore; /* ignore word 'class' */
        }

        std::sort(codes.begin(), codes.end(), [](const person& l, const person& r) {
            if (l.code != r.code) {
                return l.code > r.code;
            }
            return l.name < r.name;
        });

        for (const person& p : codes) {
            std::cout << p.name << std::endl;
        }

        std::cout << "==============================" << std::endl;
    }

    return 0;
}