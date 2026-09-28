#include "SearchEngine.h"
#include "TextProcessor.h"
#include <chrono>
#include <iostream>
#include <cassert>

SearchEngine::SearchEngine(const InvertedIndex& idx) : index(idx) {}

SearchResult SearchEngine::linearSearch(const std::string& rawQuery) const {
    SearchResult result;
    result.query = rawQuery;
    result.normalizedQuery = TextProcessor::normalizeQuery(rawQuery);
    result.method = "Linear Search";
    result.found = false;
    result.comparisons = 0;

    if (result.normalizedQuery.empty() || index.empty()) {
        result.executionTimeUs = 0.0;
        return result;
    }

    const auto& entries = index.getEntries();
    auto start = std::chrono::high_resolution_clock::now();

    // Manual Linear Search loop without std::find
    for (size_t i = 0; i < entries.size(); ++i) {
        result.comparisons++;
        if (entries[i].term == result.normalizedQuery) {
            result.found = true;
            result.documents = entries[i].documents;
            break;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> elapsed = end - start;
    result.executionTimeUs = elapsed.count();

    return result;
}

SearchResult SearchEngine::binarySearch(const std::string& rawQuery) const {
    SearchResult result;
    result.query = rawQuery;
    result.normalizedQuery = TextProcessor::normalizeQuery(rawQuery);
    result.method = "Binary Search";
    result.found = false;
    result.comparisons = 0;

    if (result.normalizedQuery.empty() || index.empty()) {
        result.executionTimeUs = 0.0;
        return result;
    }

    if (!index.isSorted()) {
        std::cerr << "[SearchEngine] Warning: Binary Search requires a sorted index!" << std::endl;
    }

    const auto& entries = index.getEntries();
    auto start = std::chrono::high_resolution_clock::now();

    int low = 0;
    int high = static_cast<int>(entries.size()) - 1;

    // Manual Binary Search without std::binary_search
    while (low <= high) {
        int mid = low + (high - low) / 2;

        result.comparisons++;
        if (entries[mid].term == result.normalizedQuery) {
            result.found = true;
            result.documents = entries[mid].documents;
            break;
        }

        result.comparisons++;
        if (entries[mid].term < result.normalizedQuery) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> elapsed = end - start;
    result.executionTimeUs = elapsed.count();

    return result;
}

std::pair<SearchResult, SearchResult> SearchEngine::compareBoth(const std::string& rawQuery) const {
    SearchResult linearRes = linearSearch(rawQuery);
    SearchResult binaryRes = binarySearch(rawQuery);

    // Consistency check: both algorithms must return identical search results
    if (linearRes.found != binaryRes.found || linearRes.documents != binaryRes.documents) {
        std::cerr << "[SearchEngine] ERROR: Inconsistency detected between Linear and Binary search results for query '"
                  << rawQuery << "'!" << std::endl;
    }

    return {linearRes, binaryRes};
}
