
import csv
import os
import sys
import time

# --- Sorting Algorithms ---

def bubble_sort(arr):
    n = len(arr)
    for i in range(n):
        for j in range(0, n - i - 1):
            if arr[j] > arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]
    return arr

def merge_sort(arr):
    if len(arr) > 1:
        mid = len(arr) // 2
        L = arr[:mid]
        R = arr[mid:]
        merge_sort(L)
        merge_sort(R)
        i = j = k = 0
        while i < len(L) and j < len(R):
            if L[i] < R[j]:
                arr[k] = L[i]
                i += 1
            else:
                arr[k] = R[j]
                j += 1
            k += 1
        while i < len(L):
            arr[k] = L[i]
            i += 1
            k += 1
        while j < len(R):
            arr[k] = R[j]
            j += 1
            k += 1
    return arr

def quick_sort(arr):
    _quick_sort_recursive(arr, 0, len(arr) - 1)
    return arr

def _quick_sort_recursive(arr, low, high):
    if low < high:
        pi = _partition(arr, low, high)
        _quick_sort_recursive(arr, low, pi - 1)
        _quick_sort_recursive(arr, pi + 1, high)

def _partition(arr, low, high):
    pivot = arr[high]
    i = low - 1
    for j in range(low, high):
        if arr[j] <= pivot:
            i += 1
            arr[i], arr[j] = arr[j], arr[i]
    arr[i + 1], arr[high] = arr[high], arr[i + 1]
    return i + 1

def heap_sort(arr):
    n = len(arr)
    for i in range(n // 2 - 1, -1, -1):
        _heapify(arr, n, i)
    for i in range(n - 1, 0, -1):
        arr[i], arr[0] = arr[0], arr[i]
        _heapify(arr, i, 0)
    return arr

def _heapify(arr, n, i):
    largest = i
    l = 2 * i + 1
    r = 2 * i + 2
    if l < n and arr[l] > arr[largest]:
        largest = l
    if r < n and arr[r] > arr[largest]:
        largest = r
    if largest != i:
        arr[i], arr[largest] = arr[largest], arr[i]
        _heapify(arr, n, largest)

def counting_sort(arr):
    if not arr:
        return []
    
    min_val = min(arr)
    max_val = max(arr)
    
    if max_val - min_val > 20_000_000:
        print(f"Warning: Skipping Counting Sort for dataset with range {max_val - min_val}", file=sys.stderr)
        return "SKIPPED"

    range_of_elements = max_val - min_val + 1
    count_arr = [0] * range_of_elements
    output_arr = [0] * len(arr)

    for i in range(len(arr)):
        count_arr[arr[i] - min_val] += 1

    for i in range(1, len(count_arr)):
        count_arr[i] += count_arr[i - 1]

    for i in range(len(arr) - 1, -1, -1):
        output_arr[count_arr[arr[i] - min_val] - 1] = arr[i]
        count_arr[arr[i] - min_val] -= 1

    return output_arr

# --- Data and Benchmarking Logic ---

def get_available_datasets():
    """Returns a list of available CSV datasets from the 'datasets' directory."""
    project_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    datasets_dir = os.path.join(project_root, 'datasets')
    try:
        return [f for f in os.listdir(datasets_dir) if f.endswith('.csv')]
    except FileNotFoundError:
        print(f"Error: The directory '{datasets_dir}' was not found.", file=sys.stderr)
        return []

def read_csv(file_path):
    """Reads a CSV file and returns its content as a list of integers."""
    try:
        with open(file_path, 'r') as f:
            reader = csv.reader(f)
            return [int(val) for val in next(reader)]
    except (IOError, StopIteration) as e:
        print(f"Error reading or parsing {file_path}: {e}", file=sys.stderr)
        return None

def run_sorting_algorithm(algo_name, dataset_path):
    """Runs a single sorting algorithm on a given dataset and prints the time taken."""
    algorithms = {
        "BubbleSort": bubble_sort,
        "MergeSort": merge_sort,
        "QuickSort": quick_sort,
        "HeapSort": heap_sort,
        "CountingSort": counting_sort
    }

    algo_func = algorithms.get(algo_name)
    if not algo_func:
        print(f"Error: Algorithm '{algo_name}' not found.", file=sys.stderr)
        return

    data = read_csv(dataset_path)
    if data is None:
        return

    # Set recursion limit for Quick Sort
    if algo_name == "QuickSort":
        sys.setrecursionlimit(max(sys.getrecursionlimit(), len(data) + 10))

    print(f"\nRunning {algo_name} on {os.path.basename(dataset_path)}...")
    
    data_copy = list(data)
    start_time = time.perf_counter()
    sorted_data = algo_func(data_copy)
    end_time = time.perf_counter()

    if isinstance(sorted_data, str) and sorted_data == "SKIPPED":
        print("Result: SKIPPED (dataset range too large for this algorithm)")
    else:
        elapsed_time = end_time - start_time
        print(f"Result: Sorted {len(sorted_data)} elements in {elapsed_time:.6f} seconds.")
