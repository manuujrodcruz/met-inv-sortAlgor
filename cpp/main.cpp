'''
#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <map>
#include <numeric>
#include <set>
#include <stack>
#include <stdexcept>
#include <dirent.h>

// --- Sorting Algorithms ---

void bubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    std::vector<int> leftArr(n1), rightArr(n2);
    for (int i = 0; i < n1; i++) leftArr[i] = arr[left + i];
    for (int j = 0; j < n2; j++) rightArr[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) arr[k++] = leftArr[i++];
        else arr[k++] = rightArr[j++];
    }
    while (i < n1) arr[k++] = leftArr[i++];
    while (j < n2) arr[k++] = rightArr[j++];
}

void mergeSort(std::vector<int>& arr) {
    if (arr.size() > 1) {
        int mid = arr.size() / 2;
        std::vector<int> L(arr.begin(), arr.begin() + mid);
        std::vector<int> R(arr.begin() + mid, arr.end());
        mergeSort(L);
        mergeSort(R);
        std::merge(L.begin(), L.end(), R.begin(), R.end(), arr.begin());
    }
}

void quickSort(std::vector<int>& arr) {
    if (arr.empty()) return;
    std::stack<std::pair<int, int>> stack;
    stack.push({0, arr.size() - 1});
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

    while (!stack.empty()) {
        auto [low, high] = stack.top();
        stack.pop();
        if (high <= low) continue;
        std::uniform_int_distribution<int> dist(low, high);
        int pivotIndex = dist(rng);
        std::swap(arr[low], arr[pivotIndex]);
        int pivot = arr[low];
        int i = low + 1, lt = low, gt = high;
        while (i <= gt) {
            if (arr[i] < pivot) std::swap(arr[lt++], arr[i++]);
            else if (arr[i] > pivot) std::swap(arr[i], arr[gt--]);
            else i++;
        }
        stack.push({low, lt - 1});
        stack.push({gt + 1, high});
    }
}

void heapify(std::vector<int>& arr, int n, int i) {
    int largest = i, left = 2 * i + 1, right = 2 * i + 2;
    if (left < n && arr[left] > arr[largest]) largest = left;
    if (right < n && arr[right] > arr[largest]) largest = right;
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

std::vector<int> countingSort(std::vector<int>& arr) {
    if (arr.empty()) return {};
    auto minmax = std::minmax_element(arr.begin(), arr.end());
    long long min_val = *minmax.first;
    long long max_val = *minmax.second;
    if (max_val - min_val > 20000000) {
        std::cerr << "Warning: Skipping Counting Sort for dataset with range " << max_val - min_val << std::endl;
        return {}; // Return empty to indicate skip
    }
    std::vector<int> count(max_val - min_val + 1, 0);
    for (int x : arr) count[x - min_val]++;
    for (size_t i = 1; i < count.size(); i++) count[i] += count[i - 1];
    std::vector<int> output(arr.size());
    for (int i = arr.size() - 1; i >= 0; i--) {
        output[count[arr[i] - min_val] - 1] = arr[i];
        count[arr[i] - min_val]--;
    }
    return output;
}


// --- Data and Benchmarking Logic ---

std::vector<std::string> getAvailableDatasets(const std::string& path) {
    std::vector<std::string> files;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path.c_str())) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string filename = ent->d_name;
            if (filename.size() > 4 && filename.substr(filename.size() - 4) == ".csv") {
                files.push_back(filename);
            }
        }
        closedir(dir);
        std::sort(files.begin(), files.end());
    } else {
        std::cerr << "Error: Could not open directory " << path << std::endl;
    }
    return files;
}

std::vector<int> readCsv(const std::string& filePath) {
    std::vector<int> data;
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return data;
    }
    std::string line, value;
    if (std::getline(file, line)) {
        std::stringstream ss(line);
        while (std::getline(ss, value, ',')) {
            try {
                data.push_back(std::stoi(value));
            } catch (const std::invalid_argument& e) {
                std::cerr << "Warning: Invalid number format in " << filePath << ": " << value << std::endl;
            } catch (const std::out_of_range& e) {
                std::cerr << "Warning: Number out of range in " << filePath << ": " << value << std::endl;
            }
        }
    }
    return data;
}

void runSortingAlgorithm(const std::string& algoName, const std::string& datasetPath) {
    std::map<std::string, std::function<void(std::vector<int>&)>> algorithms = {
        {"BubbleSort", bubbleSort},
        {"MergeSort", [](std::vector<int>& arr){ mergeSort(arr); }},
        {"QuickSort", [](std::vector<int>& arr){ quickSort(arr); }},
        {"HeapSort", heapSort}
    };
    std::map<std::string, std::function<std::vector<int>(std::vector<int>&)>> algorithms_returning_vec = {
        {"CountingSort", countingSort}
    };

    auto data = readCsv(datasetPath);
    if (data.empty()) return;

    std::cout << "
Running " << algoName << " on " << datasetPath.substr(datasetPath.find_last_of('/') + 1) << "..." << std::endl;
    
    auto data_copy = data;
    auto startTime = std::chrono::high_resolution_clock::now();

    if (algorithms.count(algoName)) {
        algorithms[algoName](data_copy);
    } else if (algorithms_returning_vec.count(algoName)) {
        data_copy = algorithms_returning_vec[algoName](data_copy);
        if (data_copy.empty()) {
             std::cout << "Result: SKIPPED (dataset range too large for this algorithm)" << std::endl;
             return;
        }
    } else {
        std::cerr << "Error: Algorithm '" << algoName << "' not found." << std::endl;
        return;
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - startTime).count();

    std::cout << "Result: Sorted " << data_copy.size() << " elements in " << std::fixed << std::setprecision(6) << duration << " seconds." << std::endl;
}


int main() {
    std::string datasetsDir = "../datasets";

    while (true) {
        std::cout << "
--- Sorting Algorithm Benchmark (C++) ---" << std::endl;

        // --- Dataset Selection ---
        std::cout << "
Available Datasets:" << std::endl;
        auto datasets = getAvailableDatasets(datasetsDir);
        if (datasets.empty()) {
            return 1;
        }
        for (size_t i = 0; i < datasets.size(); ++i) {
            std::cout << "  " << i + 1 << ": " << datasets[i] << std::endl;
        }

        int datasetChoice;
        std::cout << "Choose a dataset (number) or type 0 to exit: ";
        std::cin >> datasetChoice;
        if (std::cin.fail() || datasetChoice == 0) {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '
');
            }
            break;
        }
        if (datasetChoice < 1 || datasetChoice > static_cast<int>(datasets.size())) {
            std::cout << "Invalid choice. Please try again." << std::endl;
            continue;
        }
        std::string selectedDataset = datasets[datasetChoice - 1];
        std::string datasetPath = datasetsDir + "/" + selectedDataset;

        // --- Algorithm Selection ---
        std::vector<std::string> algorithms = {"BubbleSort", "MergeSort", "QuickSort", "HeapSort", "CountingSort"};
        std::cout << "
Available Algorithms:" << std::endl;
        for (size_t i = 0; i < algorithms.size(); ++i) {
            std::cout << "  " << i + 1 << ": " << algorithms[i] << std::endl;
        }
        
        int algoChoice;
        std::cout << "Choose an algorithm (number) or type 0 to go back: ";
        std::cin >> algoChoice;
        if (std::cin.fail() || algoChoice == 0) {
             if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '
');
            }
            continue;
        }
        if (algoChoice < 1 || algoChoice > static_cast<int>(algorithms.size())) {
            std::cout << "Invalid choice. Please try again." << std::endl;
            continue;
        }
        std::string selectedAlgo = algorithms[algoChoice - 1];

        // --- Run Benchmark ---
        runSortingAlgorithm(selectedAlgo, datasetPath);

        // --- Continue or Exit ---
        std::string another;
        while (true) {
            std::cout << "
Run another benchmark? (yes/no): ";
            std::cin >> another;
            std::transform(another.begin(), another.end(), another.begin(), ::tolower);
            if (another == "yes" || another == "no") {
                break;
            }
            std::cout << "Invalid input. Please type 'yes' or 'no'." << std::endl;
        }
        if (another == "no") {
            break;
        }
    }

    return 0;
}
''