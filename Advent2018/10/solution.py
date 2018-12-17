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


def search_message(stars, duration):
    for t in range(duration):
        coordinates = []
        for obj in stars:
            coordinates.append(obj.get_position(t))

        draw_message(coordinates)
        command = input("To exit press 'e'")
        if len(command) > 0 and command[0] != 'e':
            return


stars = read_stars(scale=0.001)
search_message(stars, 100)
