#ifndef TEXT_PROCESSOR_H
#define TEXT_PROCESSOR_H

#include <string>
#include <vector>
#include <unordered_set>

class TextProcessor {
private:
    static const std::unordered_set<std::string> STOP_WORDS;

public:
    // Convert string to lowercase
    static std::string toLowercase(const std::string& input);

    // Remove punctuation, replacing non-alphanumeric chars with space
    static std::string removePunctuation(const std::string& input);

    // Tokenize text into words
    static std::vector<std::string> tokenize(const std::string& input);

    // Full processing pipeline: lowercase -> clean punctuation -> tokenize -> filter stop words & empty strings
    static std::vector<std::string> processText(const std::string& rawText, bool filterStopWords = true);

    // Normalize a search query (lowercase & strip punctuation)
    static std::string normalizeQuery(const std::string& query);
};

#endif // TEXT_PROCESSOR_H
