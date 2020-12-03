class Solution {
public:
	int divide(int dividend, int divisor) {
		if (dividend == std::numeric_limits<int>::min() && divisor == -1) {
			return std::numeric_limits<int>::max();
		}

		long result = 0;
		bool negative = (divisor >= 0) ^ (dividend >= 0);

		long long _divisor = std::abs((long long)divisor);
		long long _dividend = std::abs((long long)dividend);

		while (_dividend >= _divisor) {
			long long div_val = 1;
			long long max_div = _divisor;

			while (_dividend >= max_div << 1) {
				max_div <<= 1;
				div_val <<= 1;
			}

			_dividend -= max_div;
			result += div_val;
		}

		return negative ? -result : result;
	}
};