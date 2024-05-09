from copy import copy as cpy

signs_by_layer = {  # signs going from up left to bottom right
    1: ('+', '-'),
    2: ('-', '-', '+', '+'),
    3: ('-', '-', '+', '-', '+', '-'),
    4: ('-', '-', '-', '-', '+', '+', '+', '-'),
    5: ('-', '+', '-', '-', '+', '+', '+', '-', '-', '+'),
    6: ('-', '+', '-', '+', '+', '-', '+', '-', '+', '+', '-', '-'),
    7: ('-', '-', '-', '+', '-', '-', '-', '+', '-', '+', '-', '-', '-', '-'),
    8: ('+', '-', '+', '+', '-', '-', '+', '+', '+', '+', '-', '-', '-', '+'),
    9: ('+', '+', '+', '-', '-', '+', '+', '+', '-', '-', '-', '+'),
    10: ('-', '+', '-', '+', '+', '+', '+', '+', '-', '-'),
    11: ('+', '+', '+', '-', '-', '-', '-', '-'),
    12: ('-', '-', '+', '+', '-', '-'),
    13: ('-', '-', '-', '+'),
    14: ('+', '+')
}

diag_size = 8
max_layer = 15
mi = 1000

out = open("output.txt", "w")


def merge(a, b):  # combine numbers from 2 neighbour layers in 1 tuple
    result = []
    if len(a) < len(b):
        for i in range(len(a)):
            result.append(a[i])
            result.append(b[i + 1])
        result.insert(0, b[0])
    else:
        for i in range(len(b)):
            result.append(b[i])
            result.append(a[i + 1])
        result.insert(0, a[0])
    return tuple(result)


def compute_pair(Si, Sj, sign):  # get value of one pair (Si * Sj)
    if sign == '+':
        return Si * Sj
    else:
        return -1 * Si * Sj


def calc_value(signs, numbers):  # get value of 2 neighbour layers combined in 1 tuple
    result = 0
    for i in range(len(signs)):
        result += compute_pair(numbers[i], numbers[i + 1], signs[i])
    return result


def get_configurations_with_minimum_value_current_layer(layer, parent_numbers):  # pure magic
    configurations_current_layer = {}
    times = layer + 2 if layer < diag_size else diag_size - (layer - diag_size)
    fors_and_configuration = """\n"""
    for i in range(1, times):
        fors_and_configuration += "for " + 'i' * i + " in (1, -1):\n" + 4 * i * ' '
    configuration = "config = ("
    for i in range(1, times):
        configuration += 'i' * i + ", "
    configuration += ')'
    fors_and_configuration += configuration + '\n' + 4 * (times - 1) * ' '
    fors_and_configuration += "value = calc_value(signs_by_layer[layer], merge(parent_numbers, config))" + '\n' + 4 * (
                times - 1) * ' '
    fors_and_configuration += "if value not in configurations_current_layer.keys():" + '\n' + 4 * times * ' '
    fors_and_configuration += "configurations_current_layer[value] = [config]" + '\n' + 4 * (times - 1) * ' '
    fors_and_configuration += "else:" + '\n' + 4 * times * ' '
    fors_and_configuration += "configurations_current_layer[value].append(config)"
    exec(fors_and_configuration)
    key = min(configurations_current_layer.keys())
    return key, tuple(configurations_current_layer[key])


class Node:
    def __init__(self, layer, energy, data, parents_stack):
        self.layer = layer
        self.energy = energy
        self.data = data
        self.children = []
        self.parents_stack = parents_stack

    def build_tree(self):
        if self.layer != max_layer:
            mimi = 1000
            children = []
            for config in self.data:
                child_params = get_configurations_with_minimum_value_current_layer(self.layer, config)
                if self.energy + child_params[0] < mimi:
                    mimi = self.energy + child_params[0]
                child = Node(self.layer + 1, self.energy + child_params[0], child_params[1], cpy(self.parents_stack))
                child.parents_stack.append(config)
                children.append(child)
            for ch in children:
                if ch.energy == mimi:
                    self.children.append(ch)
                    self.children[-1].build_tree()
        else:
            global mi
            if self.energy <= mi:
                mi = self.energy
                for i in self.data:
                    for parent in self.parents_stack:
                        out.write(str(parent) + '\n')
                    out.write(str(i) + '\n')
                    out.write('\n')
                    out.write(str(self.energy))
                    out.write('\n\n')


root = Node(3, -6, ((1, 1, 1), (-1, -1, -1)), [])
root.build_tree()

out.close()
