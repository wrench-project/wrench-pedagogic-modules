from enum import Enum

class Event():

    def __init__(self, id, start, end, num_cores_allocated):
        self.id = id
        self.start = start
        self.end = end
        self.num_cores_allocated = num_cores_allocated
        self.num_levels = 4 - num_cores_allocated + 1
        self.current_level = 0

    def __repr__(self):
        return "(task_{}, {})".format(self.id, self.current_level)


def on_segment(p1, q, p2):
    return True if q <= max(p1, p2) and q >= min(p1, p2) else False

def does_overlap(p1, q1, p2, q2):
    if p1 == p2 and q1 == q2:
        return True

    if q1 == p2 or q2 == p1:
        return False

    if on_segment(p1, p2, q1) or on_segment(p1, q2, q1) or on_segment(p2, p1, q2) or on_segment(p2, q1, q2):
        return True


class Status(Enum):
    FOUND_VALID_LAYOUT = 1
    KEEP_SEARCHING = 2

def print_path(data, next_index):
    print(data[0 : next_index + 1])


def find_layout(data):
    status = search_space(data, 0)

    if status == Status.FOUND_VALID_LAYOUT:
        print("Layout was generated..")
    elif status == Status.KEEP_SEARCHING:
        print("No solution was found..")


def search_space(data, next_index):
    if next_index == len(data) - 1:
        current_event = data[next_index]
        has_overlap = False

        for i in range(next_index):
            if does_overlap(current_event.start, current_event.end, data[i].start, data[i].end):
                if does_overlap(current_event.current_level, current_event.current_level + current_event.num_cores_allocated, data[i].current_level, data[i].current_level + data[i].num_cores_allocated):
                    has_overlap = True
                    break

        return Status.FOUND_VALID_LAYOUT if not has_overlap else Status.KEEP_SEARCHING

    if next_index < len(data) - 1:
        current_event = data[next_index]

        for level in range(current_event.num_levels):
            current_event.current_level = level

            print_path(data, next_index)

            has_overlap = False
            for i in range(next_index):
                if does_overlap(current_event.start, current_event.end, data[i].start, data[i].end):
                    if does_overlap(current_event.current_level, current_event.current_level + current_event.num_cores_allocated, data[i].current_level, data[i].current_level + data[i].num_cores_allocated):
                        has_overlap = True
                        break

            if has_overlap:
                continue

            status = search_space(data, next_index + 1)

            if status == Status.FOUND_VALID_LAYOUT:
                return Status.FOUND_VALID_LAYOUT

        return Status.KEEP_SEARCHING

if __name__=='__main__':
#    data = [
#        Event(1, 0, 1, 2),
#        Event(2, 0, 1, 2),
#        Event(3, 1, 4, 4)
#    ]

    data = [
        Event(1, 0, 1, 2),
        Event(2, 0, 1, 2),
        Event(3, 0, 1, 2)
    ]

    find_layout(data)
