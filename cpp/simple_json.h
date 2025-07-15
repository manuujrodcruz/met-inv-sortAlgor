#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

struct BenchmarkResult {
    std::string algorithm;
    std::string language;
    int arraySize;
    std::string distribution;
    long long executionTimeNanos;
    int repetition;
};

std::string escapeJsonString(const std::string& str) {
    std::string escaped;
    for (char c : str) {
        switch (c) {
            case '"': escaped += "\\\""; break;
            case '\\': escaped += "\\\\"; break;
            case '\n': escaped += "\\n"; break;
            case '\r': escaped += "\\r"; break;
            case '\t': escaped += "\\t"; break;
            default: escaped += c; break;
        }
    }
    return escaped;
}

std::string toJson(const BenchmarkResult& result) {
    std::stringstream ss;
    ss << "{\n";
    ss << "    \"algorithm\": \"" << escapeJsonString(result.algorithm) << "\",\n";
    ss << "    \"language\": \"" << escapeJsonString(result.language) << "\",\n";
    ss << "    \"arraySize\": " << result.arraySize << ",\n";
    ss << "    \"distribution\": \"" << escapeJsonString(result.distribution) << "\",\n";
    ss << "    \"executionTimeNanos\": " << result.executionTimeNanos << ",\n";
    ss << "    \"repetition\": " << result.repetition << "\n";
    ss << "}";
    return ss.str();
}

std::string toJson(const std::vector<BenchmarkResult>& results) {
    std::stringstream ss;
    ss << "[\n";
    for (size_t i = 0; i < results.size(); ++i) {
        ss << "  " << toJson(results[i]);
        if (i < results.size() - 1) {
            ss << ",";
        }
        ss << "\n";
    }
    ss << "]";
    return ss.str();
}
