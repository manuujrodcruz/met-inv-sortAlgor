package org.example;

import java.io.*;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.*;
import java.util.stream.Collectors;
import java.util.stream.Stream;

public class Main {

    // --- Sorting Algorithms ---

    public static void bubbleSort(int[] arr) {
        int n = arr.length;
        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (arr[j] > arr[j + 1]) {
                    int temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
            }
        }
    }

    public static void mergeSort(int[] arr) {
        if (arr.length > 1) {
            int mid = arr.length / 2;
            int[] L = Arrays.copyOfRange(arr, 0, mid);
            int[] R = Arrays.copyOfRange(arr, mid, arr.length);
            mergeSort(L);
            mergeSort(R);
            int i = 0, j = 0, k = 0;
            while (i < L.length && j < R.length) {
                if (L[i] < R[j]) arr[k++] = L[i++];
                else arr[k++] = R[j++];
            }
            while (i < L.length) arr[k++] = L[i++];
            while (j < R.length) arr[k++] = R[j++];
        }
    }

    public static void quickSort(int[] arr) {
        if (arr == null || arr.length == 0) return;
        Deque<Integer> stack = new ArrayDeque<>();
        stack.push(0);
        stack.push(arr.length - 1);
        Random rand = new Random();
        while (!stack.isEmpty()) {
            int high = stack.pop();
            int low = stack.pop();
            if (low >= high) continue;
            int pivotIndex = low + rand.nextInt(high - low + 1);
            int pivot = arr[pivotIndex];
            swap(arr, pivotIndex, high);
            int i = low;
            for (int j = low; j < high; j++) {
                if (arr[j] < pivot) {
                    swap(arr, i, j);
                    i++;
                }
            }
            swap(arr, i, high);
            stack.push(low);
            stack.push(i - 1);
            stack.push(i + 1);
            stack.push(high);
        }
    }

    private static void swap(int[] arr, int i, int j) {
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }

    public static void heapSort(int[] arr) {
        int n = arr.length;
        for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
        for (int i = n - 1; i > 0; i--) {
            swap(arr, 0, i);
            heapify(arr, i, 0);
        }
    }

    private static void heapify(int[] arr, int n, int i) {
        int largest = i, l = 2 * i + 1, r = 2 * i + 2;
        if (l < n && arr[l] > arr[largest]) largest = l;
        if (r < n && arr[r] > arr[largest]) largest = r;
        if (largest != i) {
            swap(arr, i, largest);
            heapify(arr, n, largest);
        }
    }

    public static int[] countingSort(int[] arr) {
        if (arr.length == 0) return new int[0];
        int min = Arrays.stream(arr).min().getAsInt();
        int max = Arrays.stream(arr).max().getAsInt();
        long range = (long)max - min + 1;
        if (range > 20_000_000) {
            System.err.println("Warning: Skipping Counting Sort for dataset with range " + range);
            return null; // Return null to indicate skip
        }
        int[] count = new int[(int)range];
        for (int value : arr) count[value - min]++;
        for (int i = 1; i < count.length; i++) count[i] += count[i - 1];
        int[] output = new int[arr.length];
        for (int i = arr.length - 1; i >= 0; i--) {
            output[count[arr[i] - min] - 1] = arr[i];
            count[arr[i] - min]--;
        }
        return output;
    }

    // --- Data and Benchmarking Logic ---

    public static List<String> getAvailableDatasets(String dirPath) {
        try (Stream<Path> stream = Files.list(Paths.get(dirPath))) {
            return stream
                .map(Path::getFileName)
                .map(Path::toString)
                .filter(name -> name.endsWith(".csv"))
                .sorted()
                .collect(Collectors.toList());
        } catch (IOException e) {
            System.err.println("Error reading datasets directory: " + e.getMessage());
            return new ArrayList<>();
        }
    }

    public static int[] readCsv(String filePath) {
        try (BufferedReader br = new BufferedReader(new FileReader(filePath))) {
            String line = br.readLine();
            return Arrays.stream(line.split(",")).mapToInt(Integer::parseInt).toArray();
        } catch (IOException | NumberFormatException e) {
            System.err.println("Error reading or parsing CSV file " + filePath + ": " + e.getMessage());
            return null;
        }
    }

    public static void runSortingAlgorithm(String algoName, String datasetPath) {
        Map<String, Consumer<int[]>> algorithms = new HashMap<>();
        algorithms.put("BubbleSort", Main::bubbleSort);
        algorithms.put("MergeSort", Main::mergeSort);
        algorithms.put("QuickSort", Main::quickSort);
        algorithms.put("HeapSort", Main::heapSort);

        Map<String, Function<int[], int[]>> algorithmsReturningArray = new HashMap<>();
        algorithmsReturningArray.put("CountingSort", Main::countingSort);

        int[] data = readCsv(datasetPath);
        if (data == null) return;

        System.out.println("\nRunning " + algoName + " on " + Paths.get(datasetPath).getFileName() + "...");
        
        int[] dataCopy = Arrays.copyOf(data, data.length);
        long startTime = System.nanoTime();

        if (algorithms.containsKey(algoName)) {
            algorithms.get(algoName).accept(dataCopy);
        } else if (algorithmsReturningArray.containsKey(algoName)) {
            dataCopy = algorithmsReturningArray.get(algoName).apply(dataCopy);
            if (dataCopy == null) {
                System.out.println("Result: SKIPPED (dataset range too large for this algorithm)");
                return;
            }
        } else {
            System.err.println("Error: Algorithm '" + algoName + "' not found.");
            return;
        }

        long endTime = System.nanoTime();
        double duration = (endTime - startTime) / 1e9;

        System.out.printf("Result: Sorted %d elements in %.6f seconds.%n", dataCopy.length, duration);
    }

    public static void main(String[] args) {
        String datasetsDir = "../../datasets"; // Relative to the execution path from target/classes
        Scanner scanner = new Scanner(System.in);

        while (true) {
            System.out.println("\n--- Sorting Algorithm Benchmark (Java) ---");

            // --- Dataset Selection ---
            System.out.println("\nAvailable Datasets:");
            List<String> datasets = getAvailableDatasets(datasetsDir);
            if (datasets.isEmpty()) {
                return;
            }
            for (int i = 0; i < datasets.size(); i++) {
                System.out.printf("  %d: %s%n", i + 1, datasets.get(i));
            }

            System.out.print("Choose a dataset (number) or type 'exit': ");
            String choiceStr = scanner.nextLine();
            if (choiceStr.equalsIgnoreCase("exit")) break;
            
            int datasetChoice;
            try {
                datasetChoice = Integer.parseInt(choiceStr);
                if (datasetChoice < 1 || datasetChoice > datasets.size()) {
                    System.out.println("Invalid choice. Please try again.");
                    continue;
                }
            } catch (NumberFormatException e) {
                System.out.println("Invalid input. Please enter a number.");
                continue;
            }
            String selectedDataset = datasets.get(datasetChoice - 1);
            String datasetPath = datasetsDir + "/" + selectedDataset;

            // --- Algorithm Selection ---
            String[] algorithms = {"BubbleSort", "MergeSort", "QuickSort", "HeapSort", "CountingSort"};
            System.out.println("\nAvailable Algorithms:");
            for (int i = 0; i < algorithms.length; i++) {
                System.out.printf("  %d: %s%n", i + 1, algorithms[i]);
            }

            System.out.print("Choose an algorithm (number) or type 'back': ");
            choiceStr = scanner.nextLine();
            if (choiceStr.equalsIgnoreCase("back")) continue;

            int algoChoice;
            try {
                algoChoice = Integer.parseInt(choiceStr);
                if (algoChoice < 1 || algoChoice > algorithms.length) {
                    System.out.println("Invalid choice. Please try again.");
                    continue;
                }
            } catch (NumberFormatException e) {
                System.out.println("Invalid input. Please enter a number.");
                continue;
            }
            String selectedAlgo = algorithms[algoChoice - 1];

            // --- Run Benchmark ---
            runSortingAlgorithm(selectedAlgo, datasetPath);

            // --- Continue or Exit ---
            while (true) {
                System.out.print("\nRun another benchmark? (yes/no): ");
                String another = scanner.nextLine().toLowerCase();
                if (another.equals("yes") || another.equals("no")) {
                    if (another.equals("no")) {
                        scanner.close();
                        return;
                    }
                    break;
                }
                System.out.println("Invalid input. Please type 'yes' or 'no'.");
            }
        }
        scanner.close();
    }
}
