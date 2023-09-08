#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>


int main() {
    std::unordered_map<std::string, std::string> dict = {
        { "at", "@" },
        { "and", "&" },
        { "one", "1" },
        { "won", "1" },
        { "to", "2" },
        { "too", "2" },
        { "two", "2" },
        { "for", "4" },
        { "four", "4" },
        { "bea", "b" },
        { "be", "b" },
        { "bee", "b" },
        { "sea", "c" },
        { "see", "c" },
        { "eye", "i" },
        { "owe", "o" },
        { "oh", "o" },
        { "are", "r" },
        { "you", "u" },
        { "why", "y" }
    };

    std::unordered_map<std::string, bool> lexes;
    for (auto& pair : dict) {
        const std::string& word = pair.first;

        std::string lex;
        for (char c : word) {
            lex.push_back(c);
            
            auto iter = lexes.find(lex);
            if (iter == lexes.cend()) {
                lexes[lex] = false;
            }
        }

        lexes[word] = true;
    }

    std::string str_n;
    std::getline(std::cin, str_n);

    int n = std::stoi(str_n);

    for (int i = 0; i < n; i++) {
        std::string line;
        std::getline(std::cin, line);

        std::string output;

        bool lex_just_added = false;
        std::string cur_lex;
        std::string orig_lex;

        for (char c : line) {
            const char orig_c = c;
            orig_lex.push_back(orig_c);

            c = std::tolower(c);
            cur_lex.push_back(c);

            auto iter = lexes.find(cur_lex);
            if (iter == lexes.cend()) {
                /* nothing found - clean first letters until we found lex */
                if (lex_just_added) {
                    cur_lex = c;
                    orig_lex = orig_c;
                    lex_just_added = false;
                }

                while (!cur_lex.empty()) {
                    if (lexes.find(cur_lex) == lexes.cend()) {
                        output.push_back(*orig_lex.begin());
                        lex_just_added = false; /* we added a letter */

                        cur_lex.erase(cur_lex.begin());
                        orig_lex.erase(orig_lex.begin());
                    }
                    else {
                        break;
                    }
                }
            }
            else if (iter->second) {
                char lex_to_add = dict[iter->first][0];
                if (std::isupper(orig_lex[0])) {
                    lex_to_add = std::toupper(lex_to_add);
                }

                if (lex_just_added) {
                    output.pop_back();
                }

                output += lex_to_add;
                lex_just_added = true;
            }
        }

        if (!orig_lex.empty() && !lex_just_added) {
            output += orig_lex;
        }

        std::cout << output << std::endl;
    }

    return 0;
}
