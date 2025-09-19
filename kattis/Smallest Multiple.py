# C++ with std::uint64_t will get overflow, easier to use python 3 to solve the problem

import sys
from math import gcd

if __name__ == "__main__":
    for line in sys.stdin:
        nums = [int(x) for x in line.split()]

        r = nums[0]
        for i in range(1, len(nums)):
            r = (r * nums[i]) // gcd(r, nums[i])

        print(r)