import re


def read_data():
    file = open('input.data')
    description = file.readline()
    result = re.search('(\d+) players; last marble is worth (\d+) points', description)

    players = int(result.group(1))
    points = int(result.group(2))

    return players, points


class node:
    def __init__(self, value):
        self.value = value
        self.next = None
        self.prev = None

    def remove(self):
        if self.prev is not None:
            self.prev.next = self.next

        if self.next is not None:
            self.next.prev = self.prev

        return self.next

    def insert_after(self, node):
        if self.next is not None:
            node.next = self.next
            self.next.prev = node

        self.next = node
        node.prev = self

    def get_prev(self, n, leaf):
        result = self
        remain = n
        while remain > 0:
            remain -= 1
            if result.prev is None:
                result = leaf
            else:
                result = result.prev

        return result


class game_field:
    def __init__(self, steps, players):
        self.step = 0
        self.marble_poisition = 1
        self.marble_value = 0
        self.last_removed = -1
        self.final_step = steps
        self.scores = [0] * players

        self.root = node(0)
        self.leaf = self.root
        self.cur_marble = self.root

        self.current_player = 0


    def __print(self):
        print("[%d]" % (self.current_player + 1), end='')
        cur_node = self.root
        while cur_node is not None:
            if cur_node.value == self.marble_value:
                print("(%2d)" % cur_node.value, end='')
            else:
                print(" %2d " % cur_node.value, end='')

            cur_node = cur_node.next

        print('\n', end='')


    def play(self):
        while self.step != self.final_step:
            self.step += 1

            # next marble
            self.marble_value += 1

            # if it is divided by 23
            if self.marble_value % 23 != 0:
                new_marble = node(self.marble_value)
                if self.cur_marble is self.leaf:
                    self.cur_marble.insert_after(new_marble)
                    self.leaf = new_marble
                    self.cur_marble = self.root

                else:
                    self.cur_marble.insert_after(new_marble)
                    self.cur_marble = new_marble.next

            else:
                marble_to_remove = self.cur_marble.get_prev(8, self.leaf)

                if marble_to_remove is self.leaf:
                    self.leaf = marble_to_remove.prev
                    self.cur_marble = self.root
                    marble_to_remove.remove()
                else:
                    self.cur_marble = marble_to_remove.remove()
                    if self.cur_marble is self.leaf:
                        self.cur_marble = self.root
                    else:
                        self.cur_marble = self.cur_marble.next

                self.scores[self.current_player] += self.marble_value + marble_to_remove.value

            #self.__print()

            self.current_player += 1
            if self.current_player >= len(self.scores):
                self.current_player = 0

        max_score = max(self.scores)
        print("Answer: ", max_score)
        print(self.scores)


players, steps = read_data()
game_field(steps, players).play()