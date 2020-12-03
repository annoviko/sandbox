class Solution {
public:
    int canCompleteCircuit(const std::vector<int>& gas, const std::vector<int>& cost) {
        int fuel = 0;
        int remain = 0;
        int start_position = 0;

        for (int i = 0; i < gas.size(); i++) {
            fuel += gas[i] - cost[i];
            if (fuel < 0) {
                remain -= fuel;

                start_position = i + 1;
                fuel = 0;
            }
        }

        if (start_position >= gas.size()) {
            return -1;
        }

        return (fuel - remain) >= 0 ? start_position : -1;
    }
};