def read_polymer():
    file = open('input.data')
    return file.readline()


class polymer_processor:
    @staticmethod
    def is_opposite(unit1, unit2):
        if unit1.lower() == unit2.lower():
            if unit1.islower() != unit2.islower():
                return True
        return False

    @staticmethod
    def polymer_reaction(polymer):
        remain_units = len(polymer)
        remain_map = [True] * remain_units

        current_index = 0
        left_border = -1
        for i in range(1, len(polymer)):
            if polymer_processor.is_opposite(polymer[current_index], polymer[i]):
                remain_map[current_index] = False
                remain_map[i] = False
                remain_units -= 2

                # go back current_index - need to check new adjacent units
                while current_index > left_border and remain_map[current_index] is False:
                    current_index -= 1

                # everything is removed before current index, just take next one
                if current_index == left_border:
                    current_index = i
                    left_border = i

            else:
                current_index = i

        return remain_units


def part1():
    polymer = read_polymer()
    print("Answer:", polymer_processor.polymer_reaction(polymer))


def part2():
    polymer = read_polymer()

    units = 'abcdefghijklmnopqrstuvwxyz'

    polymer_length = len(polymer)

    for filter in units:
        filtered_polymer = [unit for unit in polymer if unit != filter and unit.lower() != filter]
        length = polymer_processor.polymer_reaction(filtered_polymer)
        if length < polymer_length:
            polymer_length = length

    print("Answer:", polymer_length)


part1()
part2()
