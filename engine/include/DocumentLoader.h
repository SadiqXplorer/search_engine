#ifndef DOCUMENT_LOADER_H
#define DOCUMENT_LOADER_H

#include <string>
#include <vector>

struct Document {
    std::string filename;
    std::string filepath;
    std::string content;
};

class DocumentLoader {
public:
    // Load all .txt documents from a given directory path
    static std::vector<Document> loadDocumentsFromDirectory(const std::string& dirPath);

    // Load a single document by file path
    static bool loadDocument(const std::string& filePath, Document& docOut);
};

#endif // DOCUMENT_LOADER_H
