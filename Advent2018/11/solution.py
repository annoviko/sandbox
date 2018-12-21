import math

class field:
    def __init__(self, serial_number, width, height):
        self.__field = [[0 for _ in range(width)] for _ in range(height)]
        self.__serial = serial_number
        self.__width = width
        self.__height = height
        self.__create_field()


    def __compute_cell_power(self, x, y):
        rack_id = x + 10
        power_level = (rack_id * y + self.__serial) * rack_id
        hundreds = math.floor(power_level / 100) % 10
        return hundreds - 5


    def __create_field(self):
        for y in range(self.__height):
            for x in range(self.__width):
                self.__field[y][x] = self.__compute_cell_power(x, y)


    def __compute_corner(self, xc, yc, size=3):
        result = 0
        for y in range(yc, yc + size):
            for x in range(xc, xc + size):
                result += self.__field[y][x]

        return result


    def find_optimal_corner(self):
        corner = (0, 0)
        power = self.__compute_corner(0, 0)

        for y in range(0, self.__height - 3):
            for x in range(0, self.__width - 3):
                candidate_power = self.__compute_corner(x, y)
                if candidate_power > power:
                    corner = (x, y)
                    power = candidate_power

        return corner


    def __compute_step(self, xc, yc, step):
        result = self.__field[yc + step][xc + step]
        for x in range(xc, step + xc):
            result += self.__field[yc + step][x]

        for y in range(yc, step + yc):
            result += self.__field[y][xc + step]

        return result


    def __compute_corner_with_size(self, xc, yc):
        optimal_size = 1
        power = self.__field[yc][xc]

        max_xsize = self.__width - xc
        max_ysize = self.__height - yc
        max_size = min(max_xsize, max_ysize)

        candidate_power = self.__field[yc][xc]
        for size in range(1, max_size):
            candidate_power += self.__compute_step(xc, yc, size)
            if candidate_power > power:
                optimal_size = size + 1
                power = candidate_power

        return power, optimal_size


    def find_optimal_corner_with_size(self):
        power, size = self.__compute_corner_with_size(0, 0)
        corner = (0, 0, size)

        for y in range(0, self.__height):
            print(y)
            for x in range(0, self.__width):
                candidate_power, candidate_size = self.__compute_corner_with_size(x, y)
                if candidate_power > power:
                    power = candidate_power
                    corner = (x, y, candidate_power)

        return corner


    def print(self, xmax, ymax):
        for y in range(ymax):
            for x in range(xmax):
                print('%2d ' % self.__field[y][x], end='')
            print('\n', end='')



def part1():
    cells = field(1718, 300, 300)
    corner = cells.find_optimal_corner()
    print("1) Answer: %d,%d" % (corner[0], corner[1]))


def part2():
    cells = field(1718, 300, 300)
    corner = cells.find_optimal_corner_with_size()
    print("2) Answer: %d,%d,%d" % (corner[0], corner[1], corner[2]))


#part1()
part2()
