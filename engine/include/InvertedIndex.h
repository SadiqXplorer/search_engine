#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <string>
#include <vector>
#include <map>
#include <set>

struct IndexEntry {
    std::string term;
    std::vector<std::string> documents;
};

class InvertedIndex {
private:
    std::vector<IndexEntry> entries;
    bool sorted = false;

public:
    InvertedIndex() = default;

    // Add a document and its tokens to the index
    void addDocument(const std::string& docName, const std::vector<std::string>& tokens);

    // Build from map and sort entries lexicographically by term
    void sortIndex();

    // Directly set entries (useful for benchmarking synthetic datasets)
    void setEntries(const std::vector<IndexEntry>& newEntries, bool isAlreadySorted = false);

    // Accessors
    const std::vector<IndexEntry>& getEntries() const;
    size_t size() const;
    bool empty() const;
    bool isSorted() const;
    void clear();

private:
    // Temporary structure used during index building
    std::map<std::string, std::set<std::string>> tempIndex;
};

#endif // INVERTED_INDEX_H
