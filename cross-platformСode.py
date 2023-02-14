import sys
import os
from multiprocessing import Process, Queue


def count_characters(start, end, search_char, string, result_queue):
    count = 0
    for i in range(start, end):
        if ord(string[i]) > ord(search_char) and string[i].isalpha():
            count += 1
    result_queue.put(count)


if __name__ == '__main__':
    file_name = sys.argv[1]
    n_processes = int(sys.argv[2])
    search_char = sys.argv[3].lower()

    if not os.path.exists(file_name):
        print(f"File {file_name} does not exist.")
        sys.exit(1)

    with open(file_name, 'r') as file:
        string = file.readline().replace(" ", "").lower()

    if len(string) < 2:
        print("The string in the file must contain at least 2 characters.")
        sys.exit(1)

    chunk_size = len(string) // n_processes
    result_queue = Queue()
    processes = []
    start = 0
    for i in range(n_processes):
        end = start + chunk_size
        if i == n_processes - 1:
            end = len(string)
        process = Process(target=count_characters, args=(start, end, search_char, string, result_queue))
        processes.append(process)
        process.start()
        start = end

    total_count = 0
    for i in range(n_processes):
        total_count += result_queue.get()

    for process in processes:
        process.join()

    print(f"There are {total_count} letters, which are after letter \'{search_char}\' in alphabet")
