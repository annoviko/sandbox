class Solution {
public:
    int maxProfit(std::vector<int>& prices) {
        int min_price = std::numeric_limits<int>::max();
        int max_profit = 0;

        for (std::size_t i = 0; i < prices.size(); i++) {
            if (prices[i] < min_price) {
                min_price = prices[i];
            }
            else {
                int current_profit = prices[i] - min_price;
                if (current_profit > max_profit) {
                    max_profit = current_profit;
                }
            }
        }

        return max_profit;
    }
};