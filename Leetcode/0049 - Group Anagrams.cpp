class Solution {
public:
    std::vector<std::vector<std::string>> groupAnagrams(std::vector<std::string>& strs) {
        std::vector<std::unordered_map<char, int>> anagram_former;
        std::vector<std::vector<std::string>> results;

        for (const auto & word : strs) {
            std::unordered_map<char, int> word_former;
            for (const auto symbol : word) {
                if (word_former.find(symbol) == word_former.cend()) {
                    word_former[symbol] = 1;
                }
                else {
                    word_former[symbol]++;
                }
            }

            bool found_anagram = false;
            for (std::size_t i = 0; i < anagram_former.size(); i++) {
                const auto & other_former = anagram_former[i];

                if (other_former.size() != word_former.size()) {
                    continue;
                }

                bool suitable_former = true;
                for (const auto & symbol : word_former) {
                    const auto other_symbol = other_former.find(symbol.first);
                    if (other_symbol == other_former.cend() || other_symbol->second != symbol.second) {
                        suitable_former = false;
                        break;
                    }
                }

                if (suitable_former) {
                  found_anagram = true;
                  results[i].push_back(word);
                  break;
                }
            }

            if (!found_anagram) {
                anagram_former.push_back(std::move(word_former));
                results.push_back({ word });
            }
        }

        return results;
    }
};