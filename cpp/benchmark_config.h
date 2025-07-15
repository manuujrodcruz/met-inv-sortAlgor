#pragma once
#include <string>

struct BenchmarkConfig {
    // Algorithm-specific timeouts (in seconds)
    static int getTimeoutForAlgorithm(const std::string& algorithm, int size) {
        if (algorithm == "BubbleSort") {
            if (size <= 10000) return 30;
            if (size <= 50000) return 120;
            return 300; // 5 minutes for very large
        }
        if (algorithm == "CountingSort") {
            return 60; // CountingSort can be memory intensive
        }
        return 60; // Default timeout
    }
    
    // Size limits for algorithms
    static bool shouldSkipAlgorithm(const std::string& algorithm, int size) {
        if (algorithm == "BubbleSort" && size > 50000) {
            return true;
        }
        return false;
    }
    
    // Progress reporting frequency
    static int getProgressFrequency(int totalCombinations) {
        if (totalCombinations > 1000) return 50;
        if (totalCombinations > 500) return 25;
        return 10;
    }
};
