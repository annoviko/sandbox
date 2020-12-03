def read_input():
    data = open('input.data')
    return data.readlines()


class part1:
    def __init__(self):
        self._ids = read_input()
        self._two, self._three = 0, 0

    def scan_boxes(self):
        for id in self._ids:
            two, three = self._scan_box(id)
            self._two += two
            self._three += three

        return self._two * self._three

    def _scan_box(self, id):
        appearance = self._get_appearance(id)
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


    def _get_appearance(self, id):
        appearance = {}
        for symbol in id:
            if symbol in appearance:
                appearance[symbol] += 1
            else:
                appearance[symbol] = 1

        return appearance


class part2:
    def __init__(self):
        self._ids = read_input()
        self._letters = ""

    def scan_boxes(self):
        for i in range(0, len(self._ids)):
            for j in range(1, len(self._ids)):
                result = self.__scan(self._ids[i], self._ids[j])
                if result is not None:
                    return result

        return None

    def __scan(self, id1, id2):
        difference = 0
        result = ""
        for i in range(len(id1)):
            if id1[i] != id2[i]:
                difference += 1
            else:
                result += id1[i]

            if difference > 1:
                return None

        if difference == 0:
            return None

        return result


print(part1().scan_boxes())
print(part2().scan_boxes())