#ifndef SEARCH_ENGINE_H
#define SEARCH_ENGINE_H

#include "InvertedIndex.h"
#include <string>
#include <vector>
#include <utility>

struct SearchResult {
    bool found = false;
    std::string query;
    std::string normalizedQuery;
    std::string method;
    std::vector<std::string> documents;
    long long comparisons = 0;
    double executionTimeUs = 0.0; // Execution time in microseconds
};

class SearchEngine {
private:
    const InvertedIndex& index;

public:
    explicit SearchEngine(const InvertedIndex& idx);

    // Manual Linear Search O(n)
    SearchResult linearSearch(const std::string& rawQuery) const;

    // Manual Binary Search O(log n)
    SearchResult binarySearch(const std::string& rawQuery) const;

    // Run both search algorithms and return side-by-side results
    std::pair<SearchResult, SearchResult> compareBoth(const std::string& rawQuery) const;
};

#endif // SEARCH_ENGINE_H
