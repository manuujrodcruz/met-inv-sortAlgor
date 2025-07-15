import csv
import random
import os
import sys

def generate_datasets():
    """
    Generates datasets for sorting algorithm benchmarks based on specifications.
    """
    # Sizes as specified in the document
    sizes = [1000, 10000, 50000, 100000, 500000, 1000000]
    
    distributions = {
        "random_pure": generate_random,
        "unique_values": generate_unique,
        "high_repetition": generate_high_repetition,
        "partially_sorted": generate_partially_sorted,
    }
    
    # Base directory for datasets, ensuring it exists
    # The script is in python/, so we go up one level to the project root
    project_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    datasets_dir = os.path.join(project_root, 'datasets')
    
    if not os.path.exists(datasets_dir):
        os.makedirs(datasets_dir)

    # Fixed seed for reproducibility as mentioned in the document
    random.seed(42)
    
    # 32-bit integer range
    min_val = -(2**31)
    max_val = (2**31) - 1

    for size in sizes:
        for dist_name, dist_func in distributions.items():
            print(f"Generating {dist_name} dataset with size {size}...")
            # Generate data using the appropriate function
            data = dist_func(size, min_val, max_val)
            
            # Define file path and save the data
            file_path = os.path.join(datasets_dir, f"{size}_{dist_name}.csv")
            try:
                with open(file_path, 'w', newline='') as f:
                    writer = csv.writer(f)
                    writer.writerow(data)
                print(f"Successfully saved to {file_path}")
            except IOError as e:
                print(f"Error writing to file {file_path}: {e}", file=sys.stderr)
                sys.exit(1)

def generate_random(size, min_val, max_val):
    """Generates a list of random integers (pure random)."""
    return [random.randint(min_val, max_val) for _ in range(size)]

def generate_unique(size, min_val, max_val):
    """Generates a list of unique random integers."""
    if size > (max_val - min_val + 1):
        raise ValueError("Cannot generate more unique numbers than the range allows.")
    return random.sample(range(min_val, max_val), size)

def generate_high_repetition(size, min_val, max_val):
    """Generates a list with a high repetition of values."""
    # Use a small pool of values to choose from, ensuring high repetition
    num_distinct_values = max(1, size // 100)
    
    if num_distinct_values > (max_val - min_val + 1):
        # This case is highly unlikely with the given integer range
        num_distinct_values = max_val - min_val + 1

    value_pool = random.sample(range(min_val, max_val), num_distinct_values)
    return random.choices(value_pool, k=size)

def generate_partially_sorted(size, min_val, max_val):
    """Generates a partially sorted list of integers."""
    if size > (max_val - min_val + 1):
        raise ValueError("Cannot generate more unique numbers than the range allows.")
    
    # Start with a sorted list of unique numbers
    data = sorted(random.sample(range(min_val, max_val), size))
    
    # Introduce a small number of swaps (e.g., 5% of the size) to disrupt order slightly
    swaps = size // 20
    for _ in range(swaps):
        i = random.randint(0, size - 1)
        j = random.randint(0, size - 1)
        # Simple swap
        data[i], data[j] = data[j], data[i]
    return data

if __name__ == "__main__":
    generate_datasets()
