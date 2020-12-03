import re

class rectangle:
    def __init__(self, number, x, y, width, height):
        self.id = number
        self.x = x
        self.y = y
        self.width = width
        self.height = height
        self.overlaped = False


    def get_max_x(self):
        return self.x + self.width


    def get_max_y(self):
        return self.y + self.height


    @staticmethod
    def parse(string):
        result = re.search('#(\d+) @ (\d+),(\d+): (\d+)x(\d+)', string)
        number = int(result.group(1))
        x = int(result.group(2))
        y = int(result.group(3))
        width = int(result.group(4))
        height = int(result.group(5))

        return rectangle(number, x, y, width, height)


def read_data():
    file = open('input.data')
    lines = file.readlines()

    return [rectangle.parse(line) for line in lines]


def calculate_intersection_square():
    rectangles = read_data()

    xmax, ymax = 0, 0
    for rect in rectangles:
        if xmax < rect.get_max_x(): xmax = rect.get_max_x()
        if ymax < rect.get_max_y(): ymax = rect.get_max_y()

    projection = [[None for _ in range(xmax)] for _ in range(ymax)]
    square = 0

    for index_rect in range(len(rectangles)):
        rect = rectangles[index_rect]
        for i in range(rect.x, rect.x + rect.width):
            for j in range(rect.y, rect.y + rect.height):
                if projection[j][i] is not None:
                    if projection[j][i] != -1:
                        prev_rect_index = projection[j][i]
                        rectangles[prev_rect_index].overlaped = True

                        projection[j][i] = -1
                        square += 1

                    rect.overlaped = True
                else:
                    projection[j][i] = index_rect

    for rect in rectangles:
        if not rect.overlaped:
            return square, rect.id

    assert 0

print(calculate_intersection_square())