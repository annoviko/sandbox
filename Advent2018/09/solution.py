import re


def read_data():
    file = open('input.data')
    description = file.readline()
    result = re.search('(\d+) players; last marble is worth (\d+) points', description)

    players = int(result.group(1))
    points = int(result.group(2))

    return players, points


class game_field:
    def __init__(self, steps, players):
        self.step = 0
        self.marble_poisition = 1
        self.marble_value = 0
        self.last_removed = -1
        self.final_step = steps
        self.scores = [0] * players
        self.field = [0]
        self.current_player = 0


    def __print(self):
        print("[%d]" % (self.current_player + 1), end='')
        for i in range(len(self.field)):
            if self.field[i] == self.marble_value:
                print("(%2d)" % self.field[i], end='')
            else:
                print(" %2d " % self.field[i], end='')

        print('\n', end='')


    def play(self):
        while self.step != self.final_step:
            self.step += 1

            # next marble
            self.marble_value += 1

            # if it is divided by 23
            if self.marble_value % 23 != 0:
                if self.marble_poisition == len(self.field):
                    self.marble_poisition = 1
                    self.field.append(self.marble_value)

                elif self.marble_poisition < len(self.field):
                    self.field.insert(self.marble_poisition, self.marble_value)
                    self.marble_poisition += 2

                else:
                    assert False

            else:
                self.marble_poisition -= 9
                if self.marble_poisition < 0:
                    self.marble_poisition = len(self.field) - abs(self.marble_poisition)

                remove_marble_value = self.field[self.marble_poisition]
                self.scores[self.current_player] += self.marble_value + remove_marble_value
                self.field.remove(remove_marble_value)

                self.marble_poisition += 2
                if self.marble_poisition > len(self.field):
                    self.marble_poisition = self.marble_poisition - len(self.field)

            #self.__print()

            self.current_player += 1
            if self.current_player >= len(self.scores):
                self.current_player = 0

        max_score = max(self.scores)
        print("Answer: ", max_score)
        print(self.scores)


players, steps = read_data()
game_field(steps, players).play()