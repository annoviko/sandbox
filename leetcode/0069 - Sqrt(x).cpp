class Solution {
public:
    int mySqrt(int x) {
        const double delta = 0.00001;
        double difference = std::numeric_limits<double>::max();

        double prev = 0.5;
        double cur = 0;

        while (difference > delta) {
            cur = 0.5 * (prev + x / prev);
            difference = std::abs(prev - cur);

            prev = cur;
        }

        return static_cast<int>(cur);
    }
};
