def read_input():
    data = open('input.data')
    return data.readlines()


class part1:
    def __init__(self):
        self.__ids = read_input()
        self.__two, self.__three = 0, 0

    def scan_boxes(self):
        for id in self.__ids:
            two, three = self.__scan_box(id)
            self.__two += two
            self.__three += three

        return self.__two * self.__three

    def __scan_box(self, id):
        appearance = self.__get_appearance(id)
        two, three = 0, 0
        for symbol, frequency in appearance.items():
            if two and three:
                break

            if frequency == 2:
                two = True
                continue

            if frequency == 3:
                three = True
                continue

        return two, three


    def __get_appearance(self, id):
        appearance = {}
        for symbol in id:
            if symbol in appearance:
                appearance[symbol] += 1
            else:
                appearance[symbol] = 1

        return appearance


print(part1().scan_boxes())