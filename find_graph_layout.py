from enum import Enum
import json

class Event():

    def __init__(self, id, start, end, num_cores_allocated):
        self.id = id
        self.start = start
        self.end = end
        self.num_cores_allocated = num_cores_allocated
        self.num_levels = 4 - num_cores_allocated + 1 # change 4 to be whatever number of cores the host has
        self.current_position = 0

    def to_dict(self):
        return {
            "id": self.id,
            "start": self.start,
            "end": self.end,
            "num_cores": self.num_cores_allocated,
            "position": self.current_position
        }

    def __repr__(self):
        return "(task_{}, {})".format(self.id, self.current_position)


def on_segment(p1, q, p2):
    return True if q <= max(p1, p2) and q >= min(p1, p2) else False

def does_overlap(p1, q1, p2, q2):
    if p1 == p2 and q1 == q2:
        return True

    if q1 == p2 or q2 == p1:
        return False

    if on_segment(p1, p2, q1) or on_segment(p1, q2, q1) or on_segment(p2, p1, q2) or on_segment(p2, q1, q2):
        return True
    else:
        return False

class Status(Enum):
    FOUND_VALID_LAYOUT = 1
    KEEP_SEARCHING = 2

def print_path(data, next_index):
    print(data[0 : next_index + 1])


def find_layout(data):
    status = search_space(data, 0)

    if status == Status.FOUND_VALID_LAYOUT:
        print("Layout was generated..")

        with open("/Users/ryan/WebstormProjects/host_utilization/test_data.json", "w") as json_file:
            json_file.write(json.dumps([e.to_dict() for e in data], indent=4))


    elif status == Status.KEEP_SEARCHING:
        print("No solution was found..")


def search_space(data, next_index):
    if next_index == len(data) - 1:
        print_path(data, next_index)
        current_event = data[next_index]
        has_overlap = False

        for i in range(next_index):
            if does_overlap(current_event.start, current_event.end, data[i].start, data[i].end):
                if does_overlap(current_event.current_position, current_event.current_position + current_event.num_cores_allocated, data[i].current_position, data[i].current_position + data[i].num_cores_allocated):
                    has_overlap = True
                    break

        return Status.FOUND_VALID_LAYOUT if not has_overlap else Status.KEEP_SEARCHING

    if next_index < len(data) - 1:
        current_event = data[next_index]

        for level in range(current_event.num_levels):
            current_event.current_position = level

            print_path(data, next_index)

            has_overlap = False
            for i in range(next_index):
                if does_overlap(current_event.start, current_event.end, data[i].start, data[i].end):
                    if does_overlap(current_event.current_position, current_event.current_position + current_event.num_cores_allocated, data[i].current_position, data[i].current_position + data[i].num_cores_allocated):
                        has_overlap = True
                        break

            if has_overlap:
                continue

            status = search_space(data, next_index + 1)

            if status == Status.FOUND_VALID_LAYOUT:
                return Status.FOUND_VALID_LAYOUT

        return Status.KEEP_SEARCHING

if __name__=='__main__':

# some test cases

# has layout
#    data = [
#        Event(1, 0, 1, 2),
#        Event(2, 0, 1, 2),
#        Event(3, 2, 4, 4)
#    ]

# has no valid layout
#    data = [
#        Event(1, 0, 1, 2),
#        Event(2, 0, 1, 2),
#        Event(3, 0, 1, 2)
#    ]

# has layout
    data = [
        Event(1, 0, 1, 1),
        Event(2, 0, 1, 1),
        Event(3, 0, 1, 2),
        Event(4, 1, 2, 3),
        Event(5, 1, 2, 1),
        Event(6, 2, 3, 2),
        Event(7, 4, 6, 4),
        Event(8, 7, 9, 1),
        Event(9, 7, 8, 2),
        Event(10, 7, 10, 1),
        Event(11, 9, 11, 3)
    ]

    find_layout(data)
