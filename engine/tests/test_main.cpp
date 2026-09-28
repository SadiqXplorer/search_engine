#include "DocumentLoader.h"
#include "TextProcessor.h"
#include "InvertedIndex.h"
#include "SearchEngine.h"

#include <iostream>
#include <cassert>
#include <vector>
#include <string>

void testTextProcessor() {
    std::cout << "[TEST] TextProcessor..." << std::flush;
    
    // Case normalization
    assert(TextProcessor::toLowercase("Machine LEARNING!") == "machine learning!");
    
    // Tokenization and Punctuation removal
    std::vector<std::string> cleanTokens = TextProcessor::processText("c++ & AI; test.", false);
    std::vector<std::string> expectedClean = {"c", "ai", "test"};
    assert(cleanTokens == expectedClean);
    
    // Processing with stop word removal
    std::vector<std::string> tokens = TextProcessor::processText("Machine Learning is transforming AI!", true);
    // "is" is a stop word and removed
    std::vector<std::string> expected = {"machine", "learning", "transforming", "ai"};
    assert(tokens == expected);
    
    std::cout << " PASSED" << std::endl;
}

void testInvertedIndex() {
    std::cout << "[TEST] InvertedIndex & Sorting..." << std::flush;
    
    InvertedIndex index;
    index.addDocument("doc1.txt", {"machine", "learning", "ai"});
    index.addDocument("doc2.txt", {"database", "machine", "sql"});
    index.sortIndex();

    assert(index.isSorted());
    assert(index.size() == 5); // ai, database, learning, machine, sql

    const auto& entries = index.getEntries();
    assert(entries[0].term == "ai");
    assert(entries[1].term == "database");
    assert(entries[2].term == "learning");
    assert(entries[3].term == "machine");
    assert(entries[4].term == "sql");

    // Check document posting list for 'machine'
    assert(entries[3].documents.size() == 2);
    assert(entries[3].documents[0] == "doc1.txt");
    assert(entries[3].documents[1] == "doc2.txt");

    std::cout << " PASSED" << std::endl;
}

void testSearchAlgorithms() {
    std::cout << "[TEST] Linear Search vs Binary Search..." << std::flush;

    InvertedIndex index;
    index.addDocument("doc_ai.txt", {"artificial", "intelligence", "machine", "learning"});
    index.addDocument("doc_cpp.txt", {"binary", "cpp", "search", "algorithm"});
    index.addDocument("doc_db.txt", {"database", "sql", "index"});
    index.sortIndex();

    SearchEngine engine(index);

    // Test terms: beginning, middle, end, missing
    std::vector<std::string> testQueries = {
        "algorithm",    // Beginning element
        "cpp",          // Middle element
        "sql",          // End element
        "nonexistent",  // Missing term
        "MACHINE"       // Case insensitive query
    };

    for (const auto& q : testQueries) {
        SearchResult lin = engine.linearSearch(q);
        SearchResult bin = engine.binarySearch(q);

        // Result consistency check
        assert(lin.found == bin.found);
        assert(lin.documents == bin.documents);

        if (q == "nonexistent") {
            assert(!lin.found);
            assert(!bin.found);
        } else {
            assert(lin.found);
            assert(bin.found);
            assert(!lin.documents.empty());
            assert(!bin.documents.empty());
        }
    }

    std::cout << " PASSED" << std::endl;
}

void testEmptyIndex() {
    std::cout << "[TEST] Empty Index Edge Case..." << std::flush;

    InvertedIndex emptyIndex;
    emptyIndex.sortIndex();
    SearchEngine engine(emptyIndex);

    SearchResult lin = engine.linearSearch("test");
    SearchResult bin = engine.binarySearch("test");

    assert(!lin.found);
    assert(!bin.found);
    assert(lin.comparisons == 0);
    assert(bin.comparisons == 0);

    std::cout << " PASSED" << std::endl;
}

int main() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "      RUNNING C++ UNIT TEST SUITE       " << std::endl;
    std::cout << "========================================" << std::endl;

    testTextProcessor();
    testInvertedIndex();
    testSearchAlgorithms();
    testEmptyIndex();

    std::cout << "\nALL C++ UNIT TESTS PASSED SUCCESSFULLY!" << std::endl;
    std::cout << "========================================\n" << std::endl;
    return 0;
}
