#include "InvertedIndex.h"
#include <algorithm>

void InvertedIndex::addDocument(const std::string& docName, const std::vector<std::string>& tokens) {
    for (const auto& token : tokens) {
        tempIndex[token].insert(docName);
    }
    sorted = false;
}

void InvertedIndex::sortIndex() {
    entries.clear();
    entries.reserve(tempIndex.size());

    for (const auto& pair : tempIndex) {
        IndexEntry entry;
        entry.term = pair.first;
        entry.documents = std::vector<std::string>(pair.second.begin(), pair.second.end());
        entries.push_back(entry);
    }

    // Sort alphabetically by term
    std::sort(entries.begin(), entries.end(), [](const IndexEntry& a, const IndexEntry& b) {
        return a.term < b.term;
    });

    sorted = true;
}

void InvertedIndex::setEntries(const std::vector<IndexEntry>& newEntries, bool isAlreadySorted) {
    entries = newEntries;
    if (!isAlreadySorted) {
        std::sort(entries.begin(), entries.end(), [](const IndexEntry& a, const IndexEntry& b) {
            return a.term < b.term;
        });
    }
    sorted = true;
}

const std::vector<IndexEntry>& InvertedIndex::getEntries() const {
    return entries;
}

size_t InvertedIndex::size() const {
    return entries.size();
}

bool InvertedIndex::empty() const {
    return entries.empty();
}

bool InvertedIndex::isSorted() const {
    return sorted;
}

void InvertedIndex::clear() {
    entries.clear();
    tempIndex.clear();
    sorted = false;
}
