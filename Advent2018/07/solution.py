import re


part2_answer = ""


class status:
    IDLE = 1
    FINISH = 2
    PROGRESS = 3


class resource:
    def __init__(self, id_node):
        self.id = id_node
        self.prev = []
        self.next = []
        self.ready = False

    def is_ready_to_process(self):
         for node in self.prev:
             if node.ready is False:
                 return False

         return True


class worker:
    def __init__(self):
        self.node = None
        self.time_progress = 0
        self.time_required = 0

    def is_idle(self):
        return self.time_required == 0

    def set_task(self, node):
        self.time_progress = 0
        self.time_required = ord(node.id) - ord('A') + 61
        self.node = node

    def get_task(self):
        return self.node

    def simulate_step(self):
        if self.is_idle():
            return status.IDLE

        self.time_progress += 1

        if self.time_progress == self.time_required:
            self.node.ready = True
            self.time_progress = 0
            self.time_required = 0
            #print(self.node.id, end='')
            return status.FINISH

        else:
            return status.PROGRESS


def are_workers_free(workers):
    for elf in workers:
        if not elf.is_idle():
            return False

    return True


def assign_tasks_for_workers(workers, candidates):
    for elf in workers:
        if not elf.is_idle():
            continue

        if len(candidates) > 0:
            node = get_next(candidates)
            if node is not None:
                candidates.remove(node)
                elf.set_task(node)
        else:
            break


def print_worker_status(time, workers):
    print('%3d:   ' % time, end='')

    for elf in workers:
        task_id = '.'
        if not elf.is_idle():
            task_id = elf.get_task().id

        print('   %s   ' % task_id, end='')
    print('\n', end='')


def read_instruction():
    file = open('input.data')
    lines = file.readlines()

    node_storage = {}

    for string in lines:
        result = re.search('Step (\w) must be finished before step (\w) can begin.', string)
        id_node = result.group(1)
        id_next = result.group(2)

        if id_node in node_storage:
            node = node_storage[id_node]
        else:
            node = resource(id_node)
            node_storage[id_node] = node

        if id_next in node_storage:
            next = node_storage[id_next]
        else:
            next = resource(id_next)
            node_storage[id_next] = next

        node.next.append(next)
        next.prev.append(node)

    root_nodes = []
    for id_node, node in node_storage.items():
        if len(node.prev) == 0:
            root_nodes.append(node)

    return root_nodes


def get_next(candidates):
    next_node = None

    for node in candidates:
        if node.is_ready_to_process() is True:
            if next_node is None:
                next_node = node
            elif node.id < next_node.id:
                next_node = node

    #assert next_node is not None
    return next_node


def construct():
    result = ""

    root_nodes = read_instruction()

    candidates = set(root_nodes)

    while len(candidates) > 0:
        next_node = get_next(candidates)
        next_node.ready = True

        result += next_node.id

        candidates.remove(next_node)
        for node in next_node.next:
            candidates.add(node)

    return result


def simulate():
    root_nodes = read_instruction()

    candidates = set(root_nodes)
    workers = [worker() for _ in range(5)]
    assign_tasks_for_workers(workers, candidates)

    #print_worker_status(0, workers)

    current_time = 0
    while not are_workers_free(workers):
        current_time += 1

        for elf in workers:
            state = elf.simulate_step()
            if state == status.FINISH:
                next_node = elf.get_task()
                for node in next_node.next:
                    candidates.add(node)

        assign_tasks_for_workers(workers, candidates)
        #print_worker_status(current_time, workers)

    return current_time



def part1():
    print(construct())


def part2():
    print("Answer:", simulate())


#part1()
part2()