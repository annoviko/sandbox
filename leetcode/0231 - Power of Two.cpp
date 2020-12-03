class Solution {
public:
	bool isPowerOfTwo(int n) {
		return isPowerOfTwoBitCount(n);
		/* return isPowerOfTwoMultiply(n); */
	}

	bool isPowerOfTwoBitCount(int n) {
		/*
		
		1 - 0001
		2 - 0010
		4 - 0100
		8 - 1000
		16 -  0001 0000
		32 -  0010 0000
		64 -  0100 0000
		128 - 1000 0000
		256 - 0001 0000 0000
		
		*/
		std::size_t ones = 0;
		while (n) {
			if (n & 1) {
				ones++;
			}

			n = n >> 1;
		}

		return ones == 1;
	}

	bool isPowerOfTwoMultiply(int n) {
		int degree = 0;
		long long value = 0;

		for (value = 1; value < n; value *= 2) { }
		return value == n;
	}
};