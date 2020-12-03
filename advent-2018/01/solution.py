def read_input():
    input = []

    data = open('input.data')
    for value in data:
        input.append(int(value))

    return input


def part1():
    result = sum(read_input())
    return result


def part2():
    obtained_frequencies = set()
    frequencies = read_input()

    frequency = 0
    while True:
        for changes in frequencies:
            frequency += changes
            if frequency in obtained_frequencies:
                return frequency

            obtained_frequencies.add(frequency)


print(part1())
print(part2())