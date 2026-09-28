#include "TextProcessor.h"
#include <algorithm>
#include <cctype>
#include <sstream>

const std::unordered_set<std::string> TextProcessor::STOP_WORDS = {
    "a", "about", "above", "after", "again", "against", "all", "am", "an", "and",
    "any", "are", "aren't", "as", "at", "be", "because", "been", "before", "being",
    "below", "between", "both", "but", "by", "can't", "cannot", "could", "couldn't",
    "did", "didn't", "do", "does", "doesn't", "doing", "don't", "down", "during",
    "each", "few", "for", "from", "further", "had", "hadn't", "has", "hasn't",
    "have", "haven't", "having", "he", "he'd", "he'll", "he's", "her", "here",
    "here's", "hers", "herself", "him", "himself", "his", "how", "how's", "i",
    "i'd", "i'll", "i'm", "i've", "if", "in", "into", "is", "isn't", "it", "it's",
    "its", "itself", "let's", "me", "more", "most", "mustn't", "my", "myself",
    "no", "nor", "not", "of", "off", "on", "once", "only", "or", "other", "ought",
    "our", "ours", "ourselves", "out", "over", "own", "same", "shan't", "she",
    "she'd", "she'll", "she's", "should", "shouldn't", "so", "some", "such",
    "than", "that", "that's", "the", "their", "theirs", "them", "themselves",
    "then", "there", "there's", "these", "they", "they'd", "they'll", "they're",
    "they've", "this", "those", "through", "to", "too", "under", "until", "up",
    "very", "was", "wasn't", "we", "we'd", "we'll", "we're", "we've", "were",
    "weren't", "what", "what's", "when", "when's", "where", "where's", "which",
    "while", "who", "who's", "whom", "why", "why's", "with", "won't", "would",
    "wouldn't", "you", "you'd", "you'll", "you're", "you've", "your", "yours",
    "yourself", "yourselves"
};

std::string TextProcessor::toLowercase(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return result;
}

std::string TextProcessor::removePunctuation(const std::string& input) {
    std::string result;
    result.reserve(input.size());
    for (char c : input) {
        if (std::isalnum(static_cast<unsigned char>(c)) || std::isspace(static_cast<unsigned char>(c))) {
            result.push_back(c);
        } else {
            result.push_back(' ');
        }
    }
    return result;
}

std::vector<std::string> TextProcessor::tokenize(const std::string& input) {
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;
    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

std::vector<std::string> TextProcessor::processText(const std::string& rawText, bool filterStopWords) {
    std::string lower = toLowercase(rawText);
    std::string clean = removePunctuation(lower);
    std::vector<std::string> rawTokens = tokenize(clean);

    std::vector<std::string> processedTokens;
    processedTokens.reserve(rawTokens.size());

    for (const auto& token : rawTokens) {
        if (token.empty()) continue;
        if (filterStopWords && STOP_WORDS.find(token) != STOP_WORDS.end()) {
            continue;
        }
        processedTokens.push_back(token);
    }

    return processedTokens;
}

std::string TextProcessor::normalizeQuery(const std::string& query) {
    std::string lower = toLowercase(query);
    std::string clean = removePunctuation(lower);
    std::vector<std::string> tokens = tokenize(clean);
    if (tokens.empty()) return "";
    return tokens[0]; // Take first valid term for single keyword query
}
