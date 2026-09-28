#include "DocumentLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

#if defined(__has_include) && __has_include(<filesystem>) && !defined(__MINGW32__)
    #include <filesystem>
    namespace fs = std::filesystem;
    #define USE_STD_FILESYSTEM 1
#elif defined(__has_include) && __has_include(<experimental/filesystem>)
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
    #define USE_STD_FILESYSTEM 1
#elif defined(_WIN32)
    #include <windows.h>
    #define USE_WIN32_API 1
#endif

std::vector<Document> DocumentLoader::loadDocumentsFromDirectory(const std::string& dirPath) {
    std::vector<Document> documents;

#if defined(USE_STD_FILESYSTEM)
    try {
        if (!fs::exists(dirPath) || !fs::is_directory(dirPath)) {
            std::cerr << "[DocumentLoader] Warning: Directory '" << dirPath << "' does not exist or is not a directory." << std::endl;
            return documents;
        }

        for (const auto& entry : fs::directory_iterator(dirPath)) {
            #if defined(__experimental_filesystem__)
            std::string ext = entry.path().extension().string();
            std::string pathStr = entry.path().string();
            #else
            std::string ext = entry.path().extension().string();
            std::string pathStr = entry.path().string();
            #endif

            if (ext == ".txt") {
                Document doc;
                if (loadDocument(pathStr, doc)) {
                    documents.push_back(doc);
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "[DocumentLoader] Filesystem error: " << e.what() << std::endl;
    }

#elif defined(USE_WIN32_API)
    std::string searchPath = dirPath + "\\*.txt";
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        std::cerr << "[DocumentLoader] Warning: No .txt files found or directory '" << dirPath << "' inaccessible." << std::endl;
        return documents;
    }

    do {
        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            std::string fullPath = dirPath + "\\" + findData.cFileName;
            Document doc;
            if (loadDocument(fullPath, doc)) {
                documents.push_back(doc);
            }
        }
    } while (FindNextFileA(hFind, &findData) != 0);

    FindClose(hFind);
#endif

    return documents;
}

bool DocumentLoader::loadDocument(const std::string& filePath, Document& docOut) {
    std::ifstream file(filePath.c_str(), std::ios::in);

    if (!file.is_open()) {
        std::cerr << "[DocumentLoader] Error: Could not open file '" << filePath << "'" << std::endl;
        return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    // Extract filename from filepath
    size_t lastSlash = filePath.find_last_of("/\\");
    std::string filename = (lastSlash == std::string::npos) ? filePath : filePath.substr(lastSlash + 1);

    docOut.filename = filename;
    docOut.filepath = filePath;
    docOut.content = buffer.str();

    return true;
}
