cmake_minimum_required(VERSION 3.15)
project(MiniSearchEngine VERSION 1.0 LANGUAGES CXX)

# Support C++14 / C++17
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Common library source files
set(ENGINE_SOURCES
    src/DocumentLoader.cpp
    src/TextProcessor.cpp
    src/InvertedIndex.cpp
    src/SearchEngine.cpp
    src/Benchmark.cpp
)

# Core CLI/JSON Application Target
add_executable(mini_search_engine
    src/main.cpp
    ${ENGINE_SOURCES}
)

# Automated Unit Testing Target
add_executable(unit_tests
    tests/test_main.cpp
    ${ENGINE_SOURCES}
)

# Copy sample data directory to build output directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
