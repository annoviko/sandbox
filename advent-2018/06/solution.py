import re

def read_coordinates():
    file = open('input.data')
    lines = file.readlines()

    coordinates = []
    for string in lines:
        result = re.search('(\d+), (\d+)', string)
        coordinates.append([int(result.group(1)), int(result.group(2))])

    return coordinates


def create_dataset(centers):
    xmin = min(centers, key=lambda c: c[0])[0]
    ymin = min(centers, key=lambda c: c[1])[1]
    xmax = max(centers, key=lambda c: c[0])[0]
    ymax = max(centers, key=lambda c: c[1])[1]

    dataset = []
    for x in range(xmin, xmax):
        for y in range(ymin, ymax):
            dataset.append([x, y])

    return dataset, { 'xmin': xmin, 'xmax': xmax, 'ymin': ymin, 'ymax': ymax }


def manhattan_distance(point1, point2):
    return abs(point1[0] - point2[0]) + abs(point1[1] - point2[1])


def find_closest_center(point, centers):
    best_center = 0
    best_distance = manhattan_distance(point, centers[best_center])

    argue = False
    for index_center in range(1, len(centers)):
        distance = manhattan_distance(point, centers[index_center])
        if distance < best_distance:
            best_center = index_center
            best_distance = distance
            argue = False

        elif distance == best_distance:
            argue = True

    if argue: return None
    else: return best_center


def is_border_point(point, borders):
    if point[0] == borders['xmin'] or point[0] == borders['xmax'] \
            or point[1] == borders['ymin'] or point[1] == borders['ymax']:
        return True

    return False


def extract_non_infinite_clusters(dataset, centers, borders):
    clusters = {}
    for index_center in range(len(centers)):
        clusters[index_center] = []

    infinite_clusters = set()
    for point in dataset:
        index_center = find_closest_center(point, centers)
        if index_center is None:
            continue    # ambiguous, more than one owner

        if is_border_point(point, borders):
            if index_center not in infinite_clusters:
                clusters.pop(index_center)

            infinite_clusters.add(index_center)
            continue

        if index_center in infinite_clusters:
            continue    # no need to consider it

        clusters[index_center].append(point)

    return clusters


def find_biggest_size(clusters):
    max_size = 0
    for key, points in clusters.items():
        if max_size < len(points):
            max_size = len(points)

    return max_size


def find_regions(dataset, centers, limit):
    region = []
    for point in dataset:
        distance = 0
        for center in centers:
            distance += manhattan_distance(point, center)
            if distance >= limit:
                break

        if distance < limit:
            region.append(point)

    return region


def part1():
    centers = read_coordinates()
    dataset, borders = create_dataset(centers)
    clusters = extract_non_infinite_clusters(dataset, centers, borders)

    print("Answer:", find_biggest_size(clusters))


def part2():
    centers = read_coordinates()
    dataset, borders = create_dataset(centers)
    region = find_regions(dataset, centers, 10000)

    print("Answer:", len(region))


part1()
part2()