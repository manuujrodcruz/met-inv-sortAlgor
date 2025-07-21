

import os
import sys
from sorting_benchmark import get_available_datasets, run_sorting_algorithm, read_csv

def main():
    """Main function to run the sorting algorithm benchmark on all datasets with all algorithms."""
    project_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    datasets_dir = os.path.join(project_root, 'datasets')

    print("--- Sorting Algorithm Benchmark - Full Run ---")
    
    # Get all available datasets
    datasets = sorted(get_available_datasets())
    if not datasets:
        print("No datasets found in the 'datasets' directory.")
        return

    # Define all algorithms
    algorithms = ["BubbleSort", "MergeSort", "QuickSort", "HeapSort", "CountingSort"]
    
    print(f"\nFound {len(datasets)} datasets:")
    for dataset in datasets:
        print(f"  - {dataset}")
    
    print(f"\nRunning {len(algorithms)} algorithms:")
    for algo in algorithms:
        print(f"  - {algo}")
    
    print(f"\nTotal combinations: {len(datasets) * len(algorithms)}")
    print("=" * 60)

    # Run all combinations
    for dataset in datasets:
        dataset_path = os.path.join(datasets_dir, dataset)
        print(f"\n📊 Processing dataset: {dataset}")
        print("-" * 40)
        
        for algo in algorithms:
            run_sorting_algorithm(algo, dataset_path)
        
        print("-" * 40)
    
    print("\n🎉 All benchmarks completed!")
    print("=" * 60)

if __name__ == "__main__":
    main()
