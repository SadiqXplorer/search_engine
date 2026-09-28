#include "Benchmark.h"
#include <iomanip>
#include <sstream>
#include <chrono>
#include <algorithm>

InvertedIndex Benchmark::generateSyntheticIndex(size_t size) {
    InvertedIndex index;
    std::vector<IndexEntry> entries;
    entries.reserve(size);

    for (size_t i = 0; i < size; ++i) {
        IndexEntry entry;
        std::stringstream ss;
        ss << "term_" << std::setw(8) << std::setfill('0') << i;
        entry.term = ss.str();
        entry.documents = {"doc_a.txt", "doc_b.txt"};
        entries.push_back(entry);
    }

    index.setEntries(entries, true); // Entries are already lexicographically sorted by design
    return index;
}

std::vector<BenchmarkDataPoint> Benchmark::runScalingBenchmark(const std::vector<size_t>& sizes, int iterationsPerQuery) {
    std::vector<BenchmarkDataPoint> results;

    for (size_t N : sizes) {
        InvertedIndex idx = generateSyntheticIndex(N);
        SearchEngine engine(idx);
        const auto& entries = idx.getEntries();

        if (entries.empty()) continue;

        // Queries to benchmark
        std::vector<std::pair<std::string, std::string>> queryCases = {
            {"Best Case", entries[0].term},
            {"Average Case", entries[N / 2].term},
            {"Worst Case", entries[N - 1].term},
            {"Missing Term", "term_missing_xyz"}
        };

        for (const auto& qCase : queryCases) {
            BenchmarkDataPoint dp;
            dp.datasetSize = N;
            dp.queryType = qCase.first;

            const std::string& query = qCase.second;

            // Measure Linear Search over multiple iterations for precise timing
            long long totalLinComp = 0;
            auto startLin = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < iterationsPerQuery; ++i) {
                SearchResult res = engine.linearSearch(query);
                totalLinComp += res.comparisons;
            }
            auto endLin = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::micro> elapsedLin = endLin - startLin;

            dp.linearComparisons = totalLinComp / iterationsPerQuery;
            dp.linearTimeUs = elapsedLin.count() / iterationsPerQuery;

            // Measure Binary Search over multiple iterations
            long long totalBinComp = 0;
            auto startBin = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < iterationsPerQuery; ++i) {
                SearchResult res = engine.binarySearch(query);
                totalBinComp += res.comparisons;
            }
            auto endBin = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::micro> elapsedBin = endBin - startBin;

            dp.binaryComparisons = totalBinComp / iterationsPerQuery;
            dp.binaryTimeUs = elapsedBin.count() / iterationsPerQuery;

            results.push_back(dp);
        }
    }

    return results;
}

std::string Benchmark::toJSON(const std::vector<BenchmarkDataPoint>& results) {
    std::stringstream ss;
    ss << "[\n";
    for (size_t i = 0; i < results.size(); ++i) {
        const auto& dp = results[i];
        ss << "  {\n";
        ss << "    \"datasetSize\": " << dp.datasetSize << ",\n";
        ss << "    \"queryType\": \"" << dp.queryType << "\",\n";
        ss << "    \"linearComparisons\": " << dp.linearComparisons << ",\n";
        ss << "    \"linearTimeUs\": " << dp.linearTimeUs << ",\n";
        ss << "    \"binaryComparisons\": " << dp.binaryComparisons << ",\n";
        ss << "    \"binaryTimeUs\": " << dp.binaryTimeUs << "\n";
        ss << "  }" << (i + 1 < results.size() ? "," : "") << "\n";
    }
    ss << "]";
    return ss.str();
}
