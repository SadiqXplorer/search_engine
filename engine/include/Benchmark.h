#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "InvertedIndex.h"
#include "SearchEngine.h"
#include <string>
#include <vector>

struct BenchmarkDataPoint {
    size_t datasetSize;
    std::string queryType; // "Best Case", "Average Case", "Worst Case", "Missing Term"
    long long linearComparisons;
    double linearTimeUs;
    long long binaryComparisons;
    double binaryTimeUs;
};

class Benchmark {
public:
    // Generate a synthetic inverted index of size N with unique sorted terms
    static InvertedIndex generateSyntheticIndex(size_t size);

    // Run benchmark suite across dataset sizes (e.g. 100, 1000, 10000, 50000, 100000)
    static std::vector<BenchmarkDataPoint> runScalingBenchmark(const std::vector<size_t>& sizes, int iterationsPerQuery = 100);

    // Convert benchmark results to JSON string
    static std::string toJSON(const std::vector<BenchmarkDataPoint>& results);
};

#endif // BENCHMARK_H
