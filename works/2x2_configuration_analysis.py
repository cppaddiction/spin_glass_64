out = open("output.txt", "w")


def compute_pair(Si, Sj, sign):  # get value of one pair (Si * Sj)
    if sign == '+':
        return Si * Sj
    else:
        return -1 * Si * Sj


def compute_square(values, signs):  # get value of 2*2 square configuration (contains 4 pairs)
    return compute_pair(values[0], values[1], signs[0]) + compute_pair(values[1], values[2], signs[1]) + compute_pair(values[2], values[3], signs[2]) + compute_pair(values[3], values[0], signs[3])


def print_square(values, signs):  # see 2*2 square configuration
    out.write('(' + str(values[0]) + ') ' + str(signs[0]) + ' ('+str(values[1]) + ')\n')
    out.write(str(signs[3]) + '       ' + str(signs[1]) + '\n')
    out.write('(' + str(values[3]) + ') ' + str(signs[2]) + ' (' + str(values[2]) + ')\n')


'''
print_square([1, 2, 3, 4], ['+', '-', '-', '+'])
(1) + (2)
+       -
(4) - (3)
'''


def print_squares_with_specific_value(configurations_by_value, value):  # see all possible 2*2 square configurations with specific value; the value will always be one of the following: 4, 2, 0, -2, -4
    for square in configurations_by_value[value]:
        if square[1] == ('+', '-', '-', '-'):  # made-up condition
            print_square(square[0], square[1])
            out.write(str(value) + '\n\n')


class unique_element:  # for unique permutations
    def __init__(self, value, occurrences):
        self.value = value
        self.occurrences = occurrences


def perm_unique(elements):  # for unique permutations
    eset = set(elements)
    listunique = [unique_element(i, elements.count(i)) for i in eset]
    u = len(elements)
    return perm_unique_helper(listunique, [0] * u, u - 1)


def perm_unique_helper(listunique, result_list, d):  # for unique permutations
    if d < 0:
        yield tuple(result_list)
    else:
        for i in listunique:
            if i.occurrences > 0:
                result_list[d] = i.value
                i.occurrences -= 1
                for g in perm_unique_helper(listunique, result_list, d - 1):
                    yield g
                i.occurrences += 1


configurations_by_value = {}

a = ['+', '+', '+', '+']
b = ['-', '+', '+', '+']
c = ['-', '-', '+', '+']
d = ['-', '-', '-', '+']
e = ['-', '-', '-', '-']

possible_unique_sign_combinations = list(perm_unique(a)) + list(perm_unique(b)) + list(perm_unique(c)) + list(perm_unique(d)) + list(perm_unique(e))  # all possible unique sign combinations in 2*2 square configuration
print(possible_unique_sign_combinations)  # see all possible unique sign combinations in 2*2 square configuration

for sign_combination in possible_unique_sign_combinations:
    for i in [1, -1]:
        for j in [1, -1]:
            for k in [1, -1]:
                for t in [1, -1]:
                    value = compute_square([i, j, k, t], sign_combination)
                    if value not in configurations_by_value.keys():
                        configurations_by_value[value] = []
                    configurations_by_value[value].append(([i, j, k, t], sign_combination))

'''
note
minimum possible value of 2*2 square configuration = -4
the value will always be one of the following: 4, 2, 0, -2, -4
'''

print_squares_with_specific_value(configurations_by_value, 4)
print_squares_with_specific_value(configurations_by_value, 2)
print_squares_with_specific_value(configurations_by_value, 0)
print_squares_with_specific_value(configurations_by_value, -2)
print_squares_with_specific_value(configurations_by_value, -4)  # see all possible 2*2 square configurations with minimum possible value

out.close()
