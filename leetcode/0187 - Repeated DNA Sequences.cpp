#include <vector>
#include <string>
#include <unordered_map>


#define ALGORITHM_OPTIMIZED


#if defined(ALGORITHM_OPTIMIZED)
class Solution {
public:
    std::vector<std::string> findRepeatedDnaSequences(const std::string& s) {
        std::unordered_map<std::string, int> counter;
        for (int i = 10; i <= s.size(); i++) {
            std::string key = s.substr(i - 10, 10);
            counter[key]++;
        }

        std::vector<std::string> result;
        for (auto& pair : counter) {
            if (pair.second > 1) {
                result.push_back(pair.first);
            }
        }

        return result;
    }
};
#elif defined(MEMORY_OPTIMIZED)
class Solution {
public:
    std::vector<std::string> findRepeatedDnaSequences(const std::string& s) {
        std::unordered_map<char, int> dict = { { 'A', 0 }, { 'C', 1 }, { 'G', 2 }, { 'T', 3 } };
        int back_dict[4] = { 'A', 'C', 'G', 'T' };

        std::unordered_map<int, int> counter;
        for (int i = 10; i <= s.size(); i++) {
            int ikey = 0;
            for (int j = i - 10; j < i; j++) {
                ikey <<= 2;
                ikey += dict[s[j]];
            }

            counter[ikey]++;
        }

        std::vector<std::string> result;
        for (auto& pair : counter) {
            if (pair.second > 1) {
                int ikey = pair.first;

                std::string dna(10, ' ');
                for (int i = 0; i < 10; i++) {
                    int c = ikey & 3;
                    dna[9 - i] = back_dict[c];
                    ikey >>= 2;
                }

                result.push_back(dna);
            }
        }

        return result;
    }
};
#endif