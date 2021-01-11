#include <vector>
#include <string>

class Solution {
public:
    /*
    
    memory: O(1)
    time: O(n + m)
    
    */
    bool arrayStringsAreEqual(const std::vector<std::string>& word1, const std::vector<std::string>& word2) {
        std::size_t idx1 = 0, idx2 = 0;
        for (std::size_t iw1 = 0, iw2 = 0; iw1 < word1.size() || iw2 < word2.size();) {
            if (iw1 >= word1.size() || iw2 >= word2.size() ||  word1[iw1][idx1] != word2[iw2][idx2]) {
                return false;
            }

            idx1++;
            idx2++;

            if (idx1 >= word1[iw1].size()) {
                idx1 = 0;
                iw1++;
            }

            if (idx2 >= word2[iw2].size()) {
                idx2 = 0;
                iw2++;
            }
        }

        return true;
    }
};