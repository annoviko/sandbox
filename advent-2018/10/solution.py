import re


class star:
    def __init__(self, x, y, xspeed, yspeed, scale=1.0):
        self.x = x
        self.y = y
        self.xspeed = xspeed
        self.yspeed = yspeed
        self.scale = scale

    def get_position(self, t):
        return int((self.x + self.xspeed * t) * self.scale), int((self.y + self.yspeed * t) * self.scale)


def read_stars(scale):
    file = open('input.data')
    lines = file.readlines()

    stars = []
    for string in lines:
        string = string.replace(' ', '')
        result = re.search('position=<(-?\d+),(-?\d+)>velocity=<(-?\d+),(-?\d+)>', string)

        x = int(result.group(1))
        y = int(result.group(2))
        xspeed = int(result.group(3))
        yspeed = int(result.group(4))

        stars.append(star(x, y, xspeed, yspeed, scale))

    return stars


def draw_message(coordinates):
    xmin = min(coordinates, key=lambda item: item[0])[0]
    xmax = max(coordinates, key=lambda item: item[0])[0]
    ymin = min(coordinates, key=lambda item: item[1])[1]
    ymax = max(coordinates, key=lambda item: item[1])[1]

    width = xmax - xmin + 1
    height = ymax - ymin + 1

    canvas = [['.' for _ in range(width)] for _ in range(height)]
    for coord in coordinates:
        x = coord[0]
        y = coord[1]
        canvas[y - ymin][x - xmin] = '#'

    for row in canvas:
        for item in row:
            print(item, end='')
        print('\n', end='')


def has_gaps(coordinates):
    has_neighbor = [False] * len(coordinates)
    for i in range(0, len(coordinates)):
        if has_neighbor[i] is True:
            continue

        x1, y1 = coordinates[i][0], coordinates[i][1]

        for j in range(0, len(coordinates)):
            x2, y2 = coordinates[j][0], coordinates[j][1]
            if (y1 == y2 and (x1 == x2 + 1 or x1 == x2 - 1)) or \
               (x1 == x2 and (y1 == y2 + 1 or y1 == y2 - 1)) or \
               ((x1 + 1 == x2 and y1 + 1 == y2) or (x1 - 1 == x2 and y1 + 1) or (x1 + 1 == x2 and y1 - 1 == y2) or (x1 - 1 == x2 and y1 - 1 == y2)):

                has_neighbor[i] = True
                has_neighbor[j] = True
                continue

        if has_neighbor[i] is False:
            return True

    return False


def search_message(stars):
    t = 0
    while True:
        coordinates = []
        for obj in stars:
            coordinates.append(obj.get_position(t))

        if not has_gaps(coordinates):
            draw_message(coordinates)
            print("Time:", t)
            return

        t += 1


stars = read_stars(scale=1)
search_message(stars)
