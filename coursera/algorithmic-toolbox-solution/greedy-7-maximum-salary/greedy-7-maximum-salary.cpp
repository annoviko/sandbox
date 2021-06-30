#include <algorithm>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::string;

string largest_number(vector<string> a) {
    std::sort(a.rbegin(), a.rend(), [](const string& p1, const string& p2) {
        const std::size_t len = std::max(p1.size(), p2.size());
        for (std::size_t i = 0; i < len; i++) {
            char digit1 = (i < p1.size()) ? p1[i] : p2[i - p1.size()];
            char digit2 = (i < p2.size()) ? p2[i] : p1[i - p2.size()];

            if (digit1 > digit2) {
                return false;   // greater than p2
            }
            else if (digit1 < digit2) {
                return true;    // less than p2
            }
        }

        if (p1.size() == p2.size()) {
            return false; // equal to p2
        }

        // check only endings if one of them is going to be in the end
        for (auto it1 = p1.rbegin(), it2 = p2.rbegin();
            it1 != p1.rend() && it2 != p2.rend();
            it1++, it2++)
        {
            if (*it1 < *it2) {
                return true;   // less than p2
            }
            else if (*it1 > *it2) {
                return false;   // greater than p2
            }
        }

        return false; // if equal? we shouldn't be here
    });

    std::stringstream ret;
    for (size_t i = 0; i < a.size(); i++) {
        ret << a[i];
    }
    string result;
    ret >> result;
    return result;
}

void check(const string & expected, const string & actual) {
    if (expected != actual) {
        std::cout << "FAILURE: " << expected << " != " << actual << std::endl;
    }
    else {
        std::cout << "SUCCESS" << std::endl;
    }
}

void test() {
    check("232", largest_number({ "2", "23" }));
    check("19719", largest_number({ "197", "19" }));
    check("19719", largest_number({ "19", "197" }));
    check("99641", largest_number({ "9", "4", "6", "1", "9" }));
    check("923923", largest_number({ "23", "39", "92" }));
    check("89989", largest_number({ "899", "89" }));
    check("89989", largest_number({ "89", "899" }));
    check("23232", largest_number({ "23", "232" }));
    check("85885", largest_number({ "85", "858" }));
    check("20220", largest_number({ "20", "202" }));
    check("20220", largest_number({ "202", "20" }));
}

int main() {
    int n;
    std::cin >> n;
    vector<string> a(n);
    for (size_t i = 0; i < a.size(); i++) {
        std::cin >> a[i];
    }
    std::cout << largest_number(a) << std::endl;
}
