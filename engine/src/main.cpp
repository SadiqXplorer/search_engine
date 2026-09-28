#include "DocumentLoader.h"
#include "TextProcessor.h"
#include "InvertedIndex.h"
#include "SearchEngine.h"
#include "Benchmark.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

// Helper to escape strings for JSON
std::string escapeJSON(const std::string& s) {
    std::stringstream ss;
    for (char c : s) {
        if (c == '"') ss << "\\\"";
        else if (c == '\\') ss << "\\\\";
        else if (c == '\b') ss << "\\b";
        else if (c == '\f') ss << "\\f";
        else if (c == '\n') ss << "\\n";
        else if (c == '\r') ss << "\\r";
        else if (c == '\t') ss << "\\t";
        else ss << c;
    }
    return ss.str();
}

std::string searchResultToJSON(const SearchResult& res) {
    std::stringstream ss;
    ss << "{\n";
    ss << "  \"found\": " << (res.found ? "true" : "false") << ",\n";
    ss << "  \"query\": \"" << escapeJSON(res.query) << "\",\n";
    ss << "  \"normalizedQuery\": \"" << escapeJSON(res.normalizedQuery) << "\",\n";
    ss << "  \"method\": \"" << escapeJSON(res.method) << "\",\n";
    ss << "  \"comparisons\": " << res.comparisons << ",\n";
    ss << "  \"executionTimeUs\": " << res.executionTimeUs << ",\n";
    ss << "  \"documents\": [";
    for (size_t i = 0; i < res.documents.size(); ++i) {
        ss << "\"" << escapeJSON(res.documents[i]) << "\"" << (i + 1 < res.documents.size() ? ", " : "");
    }
    ss << "]\n";
    ss << "}";
    return ss.str();
}

std::string indexToJSON(const InvertedIndex& index, size_t documentCount) {
    std::stringstream ss;
    ss << "{\n";
    ss << "  \"totalTerms\": " << index.size() << ",\n";
    ss << "  \"totalDocuments\": " << documentCount << ",\n";
    ss << "  \"entries\": [\n";
    const auto& entries = index.getEntries();
    for (size_t i = 0; i < entries.size(); ++i) {
        ss << "    {\n";
        ss << "      \"term\": \"" << escapeJSON(entries[i].term) << "\",\n";
        ss << "      \"documents\": [";
        for (size_t j = 0; j < entries[i].documents.size(); ++j) {
            ss << "\"" << escapeJSON(entries[i].documents[j]) << "\"" << (j + 1 < entries[i].documents.size() ? ", " : "");
        }
        ss << "]\n";
        ss << "    }" << (i + 1 < entries.size() ? "," : "") << "\n";
    }
    ss << "  ]\n";
    ss << "}";
    return ss.str();
}

void printResultCLI(const SearchResult& res) {
    std::cout << "\n----------- " << res.method << " -----------" << std::endl;
    std::cout << "Query            : " << res.query << std::endl;
    std::cout << "Normalized Term  : " << (res.normalizedQuery.empty() ? "(none)" : res.normalizedQuery) << std::endl;
    std::cout << "Found            : " << (res.found ? "Yes" : "No") << std::endl;
    std::cout << "Matching Docs    : " << res.documents.size() << std::endl;
    if (res.found) {
        for (const auto& doc : res.documents) {
            std::cout << "  - " << doc << std::endl;
        }
    }
    std::cout << "Comparisons Made : " << res.comparisons << std::endl;
    std::cout << "Execution Time   : " << std::fixed << std::setprecision(3) << res.executionTimeUs << " μs" << std::endl;
}

int main(int argc, char* argv[]) {
    // Determine data directory path
    std::string dataDir = "data";
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--data-dir" && i + 1 < argc) {
            dataDir = argv[++i];
        }
    }

    // Ingest dataset
    std::vector<Document> docs = DocumentLoader::loadDocumentsFromDirectory(dataDir);
    InvertedIndex index;

    for (const auto& doc : docs) {
        std::vector<std::string> tokens = TextProcessor::processText(doc.content, true);
        index.addDocument(doc.filename, tokens);
    }
    index.sortIndex();

    SearchEngine engine(index);

    // JSON API Mode execution for Backend Integration
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--json") {
            std::string query = "";
            std::string mode = "both";

            for (int j = 1; j < argc; ++j) {
                std::string flag = argv[j];
                if (flag == "--query" && j + 1 < argc) query = argv[++j];
                if (flag == "--mode" && j + 1 < argc) mode = argv[++j];
            }

            if (mode == "linear") {
                SearchResult r = engine.linearSearch(query);
                std::cout << searchResultToJSON(r) << std::endl;
            } else if (mode == "binary") {
                SearchResult r = engine.binarySearch(query);
                std::cout << searchResultToJSON(r) << std::endl;
            } else { // "both"
                auto p = engine.compareBoth(query);
                std::cout << "{\n";
                std::cout << "  \"linear\": " << searchResultToJSON(p.first) << ",\n";
                std::cout << "  \"binary\": " << searchResultToJSON(p.second) << "\n";
                std::cout << "}" << std::endl;
            }
            return 0;
        }

        if (arg == "--index-json") {
            std::cout << indexToJSON(index, docs.size()) << std::endl;
            return 0;
        }

        if (arg == "--benchmark-json") {
            std::vector<size_t> sizes = {100, 1000, 10000, 50000, 100000};
            auto results = Benchmark::runScalingBenchmark(sizes, 100);
            std::cout << Benchmark::toJSON(results) << std::endl;
            return 0;
        }
    }

    // Interactive CLI Application Mode
    while (true) {
        std::cout << "\n========================================" << std::endl;
        std::cout << "        MINI SEARCH ENGINE (C++)        " << std::endl;
        std::cout << "========================================" << std::endl;
        std::cout << "Documents Indexed : " << docs.size() << std::endl;
        std::cout << "Unique Terms      : " << index.size() << std::endl;
        std::cout << "Index Status      : " << (index.isSorted() ? "SORTED (Lexicographical)" : "UNSORTED") << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "1. Linear Search" << std::endl;
        std::cout << "2. Binary Search" << std::endl;
        std::cout << "3. Compare Both (Linear vs Binary)" << std::endl;
        std::cout << "4. Index Explorer & Statistics" << std::endl;
        std::cout << "5. Run Performance Benchmark" << std::endl;
        std::cout << "6. Exit" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Enter choice (1-6): ";

        int choice = 0;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        if (choice == 6) {
            std::cout << "\nExiting Mini Search Engine. Goodbye!\n" << std::endl;
            break;
        }

        if (choice >= 1 && choice <= 3) {
            std::cout << "Enter search term: ";
            std::string query;
            std::cin >> query;

            if (choice == 1) {
                SearchResult res = engine.linearSearch(query);
                printResultCLI(res);
            } else if (choice == 2) {
                SearchResult res = engine.binarySearch(query);
                printResultCLI(res);
            } else if (choice == 3) {
                auto pair = engine.compareBoth(query);
                printResultCLI(pair.first);
                printResultCLI(pair.second);

                std::cout << "\n================ COMPARISON SUMMARY ================" << std::endl;
                std::cout << "Linear Comparisons : " << pair.first.comparisons << std::endl;
                std::cout << "Binary Comparisons : " << pair.second.comparisons << std::endl;
                std::cout << "Linear Time        : " << pair.first.executionTimeUs << " μs" << std::endl;
                std::cout << "Binary Time        : " << pair.second.executionTimeUs << " μs" << std::endl;
                long long diff = pair.first.comparisons - pair.second.comparisons;
                std::cout << "Comparison Saved   : " << diff << " operations ("
                          << (pair.second.comparisons > 0 ? (pair.first.comparisons / pair.second.comparisons) : 0) << "x reduction)" << std::endl;
                std::cout << "====================================================\n" << std::endl;
            }
        } else if (choice == 4) {
            std::cout << "\n================ INVERTED INDEX ====================" << std::endl;
            std::cout << std::left << std::setw(25) << "TERM" << "DOCUMENTS" << std::endl;
            std::cout << "----------------------------------------------------" << std::endl;
            const auto& entries = index.getEntries();
            size_t displayCount = std::min(entries.size(), static_cast<size_t>(30));
            for (size_t i = 0; i < displayCount; ++i) {
                std::stringstream docList;
                for (size_t j = 0; j < entries[i].documents.size(); ++j) {
                    docList << entries[i].documents[j] << (j + 1 < entries[i].documents.size() ? ", " : "");
                }
                std::cout << std::left << std::setw(25) << entries[i].term << docList.str() << std::endl;
            }
            if (entries.size() > 30) {
                std::cout << "... and " << (entries.size() - 30) << " more terms." << std::endl;
            }
            std::cout << "====================================================\n" << std::endl;
        } else if (choice == 5) {
            std::cout << "\nRunning synthetic scaling benchmark (N = 100 to 100,000)... Please wait..." << std::endl;
            std::vector<size_t> sizes = {100, 1000, 10000, 50000, 100000};
            auto results = Benchmark::runScalingBenchmark(sizes, 100);

            std::cout << "\n==========================================================================================" << std::endl;
            std::cout << std::left << std::setw(12) << "N SIZE"
                      << std::setw(16) << "CASE"
                      << std::setw(16) << "LIN COMPARISONS"
                      << std::setw(16) << "BIN COMPARISONS"
                      << std::setw(16) << "LIN TIME (μs)"
                      << std::setw(16) << "BIN TIME (μs)" << std::endl;
            std::cout << "------------------------------------------------------------------------------------------" << std::endl;

            for (const auto& dp : results) {
                std::cout << std::left << std::setw(12) << dp.datasetSize
                          << std::setw(16) << dp.queryType
                          << std::setw(16) << dp.linearComparisons
                          << std::setw(16) << dp.binaryComparisons
                          << std::setw(16) << std::fixed << std::setprecision(2) << dp.linearTimeUs
                          << std::setw(16) << std::fixed << std::setprecision(2) << dp.binaryTimeUs << std::endl;
            }
            std::cout << "==========================================================================================\n" << std::endl;
        }
    }

    return 0;
}
