class Solution {
public:
    int totalNumbers(const std::vector<int>& digits) {
        std::unordered_set<int> values;
        for (int i = 0; i < digits.size(); i++) {
            for (int j = 0; j < digits.size(); j++) {
                if (i == j) {
                    continue;
                }

                for (int k = 0; k < digits.size(); k++) {
                    if ((k == i) || (k == j)) {
                        continue;
                    }

                    int value = digits[i] * 100 + digits[j] * 10 + digits[k];
                    if ((value >= 100) && (value % 2 == 0)) {
                        values.insert(value);
                    }
                }
            }
        }

        return values.size();
    }
};