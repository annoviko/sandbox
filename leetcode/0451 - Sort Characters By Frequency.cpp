class Solution {
public:
    string frequencySort(string s) {
        int max_freq = 0;
        int counter[255] = { 0 };
        for (auto& elem : s) {
            counter[elem]++;
            max_freq = std::max(max_freq, counter[elem]);
        }

        std::vector<std::vector<int>> freq(max_freq + 1);  /* [3] : [c, a] */
        for (int i = 0; i <= 'z'; i++) {
            int freq_value = counter[i];
            if (freq_value > 0) {
                freq[freq_value].push_back(i);
            }
        }

        std::string result;
        for (int i = freq.size() - 1; i >= 0; i--) {
            for (const auto & elem : freq[i]) {
                result += std::string(i, (char) elem);
            }
        }

        return result;
    }
};