class Solution {
public:
    int nthUglyNumber(int n) {
        if (n < 1) { return 0; }

        std::vector<int> values = { 1 };
        std::size_t k2 = 0, k3 = 0, k5 = 0;

        int result = 1;
        for (int i = result; i < n; i++) {
            result = std::min(values[k2] * 2, std::min(values[k3] * 3, values[k5] * 5));
            values.push_back(result);

            if (result == values[k2] * 2) k2++;
            if (result == values[k3] * 3) k3++;
            if (result == values[k5] * 5) k5++;
        }

        return result;
    }
};
