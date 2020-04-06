#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>


class Solution {
public:
#if defined(HASH_PATTERNS)
    std::vector<std::vector<std::string>> groupAnagrams(const std::vector<std::string>& strs) {
        std::vector<std::vector<std::string>> result = { };
        std::vector<std::unordered_map<char, int>> patterns;

        for (const auto & value : strs) {   // O(N)
            std::unordered_map<char, int> pattern;
            for (const char sym : value) {  // O(K)
                pattern[sym]++;
            }

            bool found = false;
            for (std::size_t i = 0; i < result.size(); i++) {   // O(N)
                if ((patterns[i].size() == pattern.size()) && (patterns[i] == pattern)) {
                    result[i].push_back(value);
                    found = true;
                    break;
                }
            }

            if (!found) {
                result.push_back({ value });
                patterns.push_back(std::move(pattern));
            }
        }

        // Total O(N^2)

        return result;
    }
#else
    std::vector<std::vector<std::string>> groupAnagrams(const std::vector<std::string>& strs) {
        std::unordered_map<std::string, std::vector<std::string>> hash_groups;
        for (const auto & value : strs) {   // O(N)
            std::vector<int> counter(26, 0);
            for (const char sym : value) {  // O(K)
                counter[sym - 'a']++;
            }

            std::string hash;
            for (const int counter_value : counter) {   // O(K)
                hash += "#" + std::to_string(counter_value);
            }

            hash_groups[hash].push_back(value);     // O(1)
        }

        // Total O(KN)

        std::vector<std::vector<std::string>> result;
        for (auto & group : hash_groups) {
            result.push_back(std::move(group.second));
        }

        return result;
    }
#endif
};