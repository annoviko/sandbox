#!/usr/bin/python
# -*- coding: utf-8 -*-

import time

from collections import namedtuple
Item = namedtuple("Item", ['index', 'value', 'weight'])


class dp_solver:
    def __init__(self, items, capacity):
        self.__items = items
        self.__capacity = capacity

    def solve(self):
        self.__items.sort(key=lambda item: item.weight)

        dp = [[0]*(self.__capacity + 1) for _ in range(len(self.__items))]
        for i in range(len(self.__items)):
            for k in range(self.__capacity + 1):
                lvalue = 0
                if i > 0:
                    lvalue = dp[i - 1][k]

                rvalue = 0
                if k >= self.__items[i].weight:
                    rvalue = dp[i - 1][k - self.__items[i].weight] + self.__items[i].value

                dp[i][k] = max(lvalue, rvalue)

        taken = [0]*len(self.__items)

        current_weight = self.__capacity

        for i in range(len(self.__items) - 1, -1, -1):
            item = self.__items[i]
            if i > 0:
                if dp[i][current_weight] != dp[i - 1][current_weight]:
                    taken[item.index] = 1
                    current_weight -= item.weight
                    assert current_weight >= 0
            else:
                if dp[i][current_weight] != 0:
                    taken[item.index] = 1

        assert_section(taken, self.__items, self.__capacity)

        return dp[-1][-1], taken


class greedy_solver:
    def __init__(self, items, capacity):
        self.__items = items
        self.__capacity = capacity

    def solve(self):
        price_per_kilo = [item.value / item.weight for item in self.__items]

        optimal_weight = 0
        taken = [0]*len(self.__items)

        self.__items.sort(key=lambda item: price_per_kilo[item.index], reverse=True)
        for item in self.__items:
            current_weight = item.weight + optimal_weight
            if current_weight > self.__capacity:
                continue
            else:
                optimal_weight = current_weight
                taken[item.index] = 1

            if optimal_weight == self.__capacity:
                break

        assert_section(taken, self.__items, self.__capacity)

        return optimal_weight, taken


class branch_status:
    def __init__(self, volume=0, possible_value=0):
        self.cur_value = 0
        self.available_volume = volume
        self.possible_value = possible_value


class bound_branch_solver:
    def __init__(self, items, capacity):
        self.__items = items
        self.__capacity = capacity
        self.__start_time = 0

    def __traverse(self, taken, index, go_left, cur_status, cur_border):
        cur_time = time.time()
        if cur_time - self.__start_time > 10:
            return None

        next_status = branch_status()
        next_status.cur_value = cur_status.cur_value
        next_status.available_volume = cur_status.available_volume
        next_status.possible_value = cur_status.possible_value

        if index != -1:
            if go_left is True:
                if cur_status.available_volume < self.__items[index].weight:
                    return None     # Weight is bigger then capacity in this branch - skip it

                next_status.cur_value += self.__items[index].value
                next_status.available_volume -= self.__items[index].weight

                taken[index] = 1

            else:
                taken[index] = 0

            next_status.possible_value -= self.__items[index].value

            if next_status.possible_value + next_status.cur_value < cur_border:
                return None     # Potential value is already less then border in this branch - skip it

        best_result = None
        if index + 1 < len(self.__items):
            left_result = self.__traverse(taken[:], index + 1, True, next_status, cur_border)
            if left_result is not None:
                if left_result[0].cur_value > cur_border:
                    cur_border = left_result[0].cur_value

                best_result = left_result

                if best_result[0].available_volume == 0:
                    return best_result

            right_result = self.__traverse(taken[:], index + 1, False, next_status, cur_border)
            if right_result is not None:
                if best_result is None:
                    best_result = right_result

                elif best_result[0].cur_value < right_result[0].cur_value:
                    best_result = right_result

                    if best_result[0].available_volume == 0:
                        return best_result

        else:   # leaf is reached
            best_result = (next_status, taken)

        return best_result


    def solve(self):
        self.__start_time = time.time()

        border, greedy_taken = greedy_solver(self.__items, self.__capacity).solve()

        taken = [0]*len(self.__items)

        possible_value = sum(item.value for item in self.__items)

        initial_status = branch_status(self.__capacity, possible_value)

        result = self.__traverse(taken, -1, None, initial_status, border)

        if result is None:
            result = (border, greedy_taken)
        else:
            result = self.__capacity - result[0].available_volume, result[1]

        assert_section(result[1], self.__items, self.__capacity)

        return result[0], result[1]


def assert_section(taken, items, capacity):
    weight = 0
    for item in items:
        if taken[item.index] == 1:
            weight += item.weight

    if weight > capacity:
        raise AssertionError("Capacity %d is less then value %d" % (capacity, weight))


def solve_it(input_data):
    # Modify this code to run your optimization algorithm

    # parse the input
    lines = input_data.split('\n')

    firstLine = lines[0].split()
    item_count = int(firstLine[0])
    capacity = int(firstLine[1])

    items = []

    for i in range(1, item_count+1):
        line = lines[i]
        parts = line.split()
        items.append(Item(i-1, int(parts[0]), int(parts[1])))

    # a trivial algorithm for filling the knapsack
    # it takes items in-order until the knapsack is full
    if capacity * len(items) < 500000000:
        #print("Run dynamic programming solver")
        optimal_value, taken = dp_solver(items, capacity).solve()
        real_optimal = 1
    else:
        import sys
        if len(items) < sys.getrecursionlimit() - 10:
            #print("Run bound branch solver")
            optimal_value, taken = bound_branch_solver(items, capacity).solve()

            #greedy = greedy_solver(items, capacity).solve()
            #print("B = %d, G = %d" % (optimal_value, greedy[0]))
        else:
            #print("Run greedy solver")
            optimal_value, taken = greedy_solver(items, capacity).solve()

        real_optimal = 0

    # prepare the solution in the specified output format
    output_data = str(optimal_value) + ' ' + str(real_optimal) + '\n'
    output_data += ' '.join(map(str, taken))
    return output_data


def test():
    #run('data/ks_10000_0')
    #return

    import os

    files = os.listdir('data/')
    for file in files:
        print("Analyse", file)
        run('data/' + file)


def run(file_location):
    with open(file_location, 'r') as input_data_file:
        input_data = input_data_file.read()
    print(solve_it(input_data))


if __name__ == '__main__':
    if 0:
        test()
    else:
        import sys
        if len(sys.argv) > 1:
            file_location = sys.argv[1].strip()
            run(file_location)
        else:
            print('This test requires an input file.  Please select one from the data directory. (i.e. python solver.py ./data/ks_4_0)')

