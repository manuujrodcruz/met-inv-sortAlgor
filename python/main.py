

import os
import sys
from sorting_benchmark import get_available_datasets, run_sorting_algorithm, read_csv

def main():
    """Main function to run the sorting algorithm benchmark CLI."""
    project_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    datasets_dir = os.path.join(project_root, 'datasets')

    while True:
        print("\n--- Sorting Algorithm Benchmark ---")
        
        # --- Dataset Selection ---
        print("\nAvailable Datasets:")
        datasets = sorted(get_available_datasets())
        if not datasets:
            print("No datasets found in the 'datasets' directory.")
            return

        for i, dataset in enumerate(datasets):
            print(f"  {i + 1}: {dataset}")
        
        try:
            choice = input("Choose a dataset (number) or type 'exit': ")
            if choice.lower() == 'exit':
                break
            dataset_index = int(choice) - 1
            if not 0 <= dataset_index < len(datasets):
                print("Invalid choice. Please try again.")
                continue
            selected_dataset = datasets[dataset_index]
            dataset_path = os.path.join(datasets_dir, selected_dataset)
        except ValueError:
            print("Invalid input. Please enter a number.")
            continue

        # --- Algorithm Selection ---
        algorithms = ["BubbleSort", "MergeSort", "QuickSort", "HeapSort", "CountingSort"]
        print("\nAvailable Algorithms:")
        for i, algo in enumerate(algorithms):
            print(f"  {i + 1}: {algo}")

        try:
            choice = input("Choose an algorithm (number) or type 'back': ")
            if choice.lower() == 'back':
                continue
            algo_index = int(choice) - 1
            if not 0 <= algo_index < len(algorithms):
                print("Invalid choice. Please try again.")
                continue
            selected_algo = algorithms[algo_index]
        except ValueError:
            print("Invalid input. Please enter a number.")
            continue

        # --- Run Benchmark ---
        run_sorting_algorithm(selected_algo, dataset_path)

        # --- Continue or Exit ---
        while True:
            another = input("\nRun another benchmark? (yes/no): ").lower()
            if another in ["yes", "no"]:
                break
            print("Invalid input. Please type 'yes' or 'no'.")
        
        if another == "no":
            break

if __name__ == "__main__":
    main()
