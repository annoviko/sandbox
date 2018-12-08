import re
import datetime


def find_sleepest(log):
    sleepest_guard, sleep_duration = None, -1

    for guard, entries in log.entries.items():
        duration = 0

        time_asleep = None
        for guard_entry in entries:
            if guard_entry.action == 's': time_asleep = guard_entry.time
            elif guard_entry.action == 'w':
                duration += (guard_entry.time - time_asleep).total_seconds() / 60

        if duration > sleep_duration:
            sleepest_guard = guard
            sleep_duration = duration

    return sleepest_guard, sleep_duration


def find_likely_sleep_time_median(guard_journal):
    sleep_minutes = []
    for guard_entry in guard_journal:
        if guard_entry.action == 's':
            shift_time = datetime.datetime(guard_entry.time.year,
                                           guard_entry.time.month,
                                           guard_entry.time.day,
                                           0, 0, 0, 0)

            sleep_minute = (guard_entry.time - shift_time).total_seconds() / 60
            sleep_minutes.append(sleep_minute)

    sleep_minutes.sort()
    return sleep_minutes[int(len(sleep_minutes) / 2)]


def find_likely_sleep_time(guard_journal):
    sleep_minutes = [0] * (24 * 60)
    start_sleep = None
    for guard_entry in guard_journal:
        if guard_entry.action == 's':
            start_sleep = guard_entry.time.hour * 60 + guard_entry.time.minute
        elif guard_entry.action == 'w':
            assert start_sleep is not None
            stop_sleep = guard_entry.time.hour * 60 + guard_entry.time.minute
            for i in range(start_sleep, stop_sleep):
                sleep_minutes[i] += 1

    amount_fall_asleep = max(sleep_minutes)
    return sleep_minutes.index(amount_fall_asleep), amount_fall_asleep


def find_likely_sleep_time_among_all(log):
    probable_sleep_minute = 0
    best_guard = None
    max_amount_fall = -1

    for guard, guard_entry in log.entries.items():
        sleep_minute, amount = find_likely_sleep_time(guard_entry)
        if max_amount_fall < amount:
            best_guard = guard
            max_amount_fall = amount
            probable_sleep_minute = sleep_minute

    return best_guard, probable_sleep_minute


class entry:
    def __init__(self, action, time):
        self.action = action
        self.time = time


class journal:
    def __init__(self):
        self.entries = {}

    def load(self):
        file = open('input.data')
        lines = file.readlines()

        guard = None

        time_sorted_journal = []
        for string in lines:
            result = re.search('\[(\d+)-(\d+)-(\d+) (\d+):(\d+)\] (.*)', string)
            year = int(result.group(1))
            month = int(result.group(2))
            day = int(result.group(3))
            hour = int(result.group(4))
            minute = int(result.group(5))

            time = datetime.datetime(year, month, day, hour, minute, 0, 0)
            string_action = result.group(6)

            time_sorted_journal.append(entry(string_action, time))


        time_sorted_journal.sort(key=lambda e: e.time)

        for sorted_entry in time_sorted_journal:
            string_action = sorted_entry.action
            time = sorted_entry.time

            action = None

            if string_action.find('wakes up') != -1: action = 'w'
            elif string_action.find('falls asleep') != -1: action = 's'
            elif string_action.find('Guard') != -1:
                result = re.search('Guard #(\d+) begins shift', string_action)
                guard = int(result.group(1))
            else:
                assert 0

            if action is not None:
                assert guard is not None
                assert time is not None
                self.entries[guard].append(entry(action, time))
            elif guard not in self.entries:
                self.entries[guard] = []

        return self



def part1():
    log = journal().load()
    guard, duration = find_sleepest(log)

    print("Guard and sleep duration:", guard, duration)

    sleep_minute, amount_fall_asleep = find_likely_sleep_time(log.entries[guard])

    print("Most probable time sleep:", sleep_minute)
    print("Answer (Part 1):", guard * sleep_minute)


def part2():
    log = journal().load()

    guard, sleep_minute = find_likely_sleep_time_among_all(log)

    print("Most probable time sleep:", sleep_minute)
    print("Answer (Part 2):", guard * sleep_minute)


part1()
part2()
