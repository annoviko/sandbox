def read_data():
    file = open('input.data')
    data = file.readline().split(' ')
    return [int(value) for value in data]


class tree_calculator:
    def __init__(self, data):
        self.data = data
        self.position = 0
        self.meta_sum = 0
        self.root_value = 0


    def calculate_meta_sum(self):
        amount_nodes = self.data[self.position]
        meta_nodes = self.data[self.position + 1]

        self.position += 2

        for _ in range(amount_nodes):
            self.calculate_meta_sum()

        for _ in range(meta_nodes):
            self.meta_sum += self.data[self.position]
            self.position += 1

        return self.meta_sum


    def calculate_node_value(self):
        amount_nodes = self.data[self.position]
        amount_meta_nodes = self.data[self.position + 1]

        self.position += 2

        child_values = [0] * amount_nodes
        for index_child in range(amount_nodes):
            child_values[index_child] = self.calculate_node_value()

        meta_values = [0] * amount_meta_nodes
        for index_meta in range(amount_meta_nodes):
            meta_values[index_meta] = self.data[self.position]
            self.position += 1

        node_value = 0
        if amount_nodes > 0:
            for index_child in meta_values:
                if index_child - 1 < amount_nodes:
                    node_value += child_values[index_child - 1]
        else:
            for meta_value in meta_values:
                node_value += meta_value

        return node_value



def part1():
    result = tree_calculator(read_data()).calculate_meta_sum()
    print("Answer:", result)


def part2():
    result = tree_calculator(read_data()).calculate_node_value()
    print("Answer:", result)


part1()
part2()
