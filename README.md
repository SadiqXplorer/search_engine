# Mini Search Engine — Linear Search vs Binary Search Performance Analyzer

A high-performance, educational search engine software built in **C++17**, **Node.js (Express)**, **React**, **TypeScript**, and **Vite**.

The engine processes text documents, normalizes tokens, constructs an **Inverted Index**, sorts the index lexicographically, and directly compares manual **Linear Search** $\mathcal{O}(n)$ against **Binary Search** $\mathcal{O}(\log n)$ on identical indexed dataset states.

---

## 🌟 Core Architecture & Pipeline Flow

```
+----------------+      +------------------+      +----------------+      +----------------+
| Text Documents | ---> | TextProcessor    | ---> | Inverted Index | ---> | Lexicographical|
| (*.txt files)  |      | (Clean & Token)  |      | Construction   |      | Index Sorting  |
+----------------+      +------------------+      +----------------+      +----------------+
                                                                                 |
                                                                                 v
+----------------+      +------------------+      +----------------+      +----------------+
| Search Results | <--- | Comparisons      | <--- | Query Execution| <--- | User Search    |
| & Dashboard UI |      | & Timing Profr   |      | (Linear/Binary)|      | Input          |
+----------------+      +------------------+      +----------------+      +----------------+
```

---

## 🔍 How the Inverted Index Works

An **Inverted Index** maps words/terms to the specific document filenames where they appear (their posting list).

### Example:
Given two text files:
* `ai.txt`: `"Machine learning powers artificial intelligence"`
* `cpp.txt`: `"C++ is a high performance machine learning language"`

### Preprocessed Tokens & Postings:
```
artificial   -> [ai.txt]
cpp          -> [cpp.txt]
intelligence -> [ai.txt]
language     -> [cpp.txt]
learning     -> [ai.txt, cpp.txt]
machine      -> [ai.txt, cpp.txt]
performance  -> [cpp.txt]
```

### Why Lexicographical Sorting is Required
Binary Search relies on the **order property**. Before performing Binary Search, the array of index entries (`std::vector<IndexEntry>`) is explicitly sorted alphabetically by `term`. Without sorting, Binary Search cannot divide and conquer the search space.

---

## 📊 Algorithmic Comparison: Linear vs Binary Search

| Metric | Linear Search | Binary Search |
| :--- | :--- | :--- |
| **Time Complexity (Worst/Avg)** | $\mathcal{O}(n)$ | $\mathcal{O}(\log_2 n)$ |
| **Best Case** | $\mathcal{O}(1)$ (first term) | $\mathcal{O}(1)$ (middle term) |
| **Prerequisite** | None (Unsorted or Sorted data) | Sorted Index Array Required |
| **100,000 Terms Comparison Count** | Up to **100,000 comparisons** | At most **17 comparisons** ($\lceil \log_2 100,000 \rceil$) |
| **Implementation** | Manual loop iteration | Manual `low`, `high`, `mid` index pointer reduction |

---

## 📁 Repository Structure

```
search_engine/
├── engine/                   # Native C++ Core Engine
│   ├── include/
│   │   ├── DocumentLoader.h  # File reader & dynamic directory scanner
│   │   ├── TextProcessor.h   # Lowercase, punctuation cleaner, stop words
│   │   ├── InvertedIndex.h   # Lexicographical inverted index data structure
│   │   ├── SearchEngine.h    # Manual Linear & Binary search algorithms
│   │   └── Benchmark.h       # Synthetic dataset benchmark engine
│   ├── src/
│   │   ├── DocumentLoader.cpp
│   │   ├── TextProcessor.cpp
│   │   ├── InvertedIndex.cpp
│   │   ├── SearchEngine.cpp
│   │   ├── Benchmark.cpp
│   │   └── main.cpp          # CLI & JSON API entrypoint
│   ├── data/                 # Text document collection (.txt files)
│   ├── tests/
│   │   └── test_main.cpp     # Automated unit test suite
│   └── CMakeLists.txt
├── backend/                  # Node.js Express API Bridge
│   ├── package.json
│   └── server.js             # Subprocess IPC wrapper for C++ binary
├── frontend/                 # React + TypeScript + Vite UI
│   ├── src/
│   │   ├── components/       # SearchBar, ResultCard, Dashboard, IndexExplorer
│   │   ├── App.tsx
│   │   └── main.tsx
│   ├── package.json
│   └── vite.config.ts
└── README.md
```

---

## 🛠️ Prerequisites & Build Instructions

### Requirements
* **C++ Compiler**: GCC 6.3+ / Clang 10+ / MSVC 2019+
* **Build System**: CMake 3.15+
* **Runtime**: Node.js v18+ & npm

---

### Step 1: Compile the Native C++ Engine

```bash
cd engine

# Configure build with CMake
cmake -B build -S .

# Build C++ executables (mini_search_engine & unit_tests)
cmake --build build
```

---

### Step 2: Run C++ Unit Tests

```bash
cd engine
./build/unit_tests
```

---

### Step 3: Run Interactive C++ CLI Application

```bash
cd engine
./build/mini_search_engine
```

#### JSON IPC Commands:
```bash
# Search query via JSON
./build/mini_search_engine --json --query "machine" --mode "both"

# Export inverted index JSON
./build/mini_search_engine --index-json

# Export scaling benchmarks JSON (N = 100 to 100,000)
./build/mini_search_engine --benchmark-json
```

---

### Step 4: Run Backend Express API

```bash
cd backend
npm install
npm start
```
*API Server listens at `http://localhost:5000`.*

---

### Step 5: Run React Web Frontend

```bash
cd frontend
npm install
npm run dev
```
*Open `http://localhost:3000` in your browser.*

---

## 🌐 API Endpoints

* `GET /api/health`: Returns server status and C++ binary path.
* `GET /api/search?q=<term>&mode=both`: Runs C++ search engine for exact query. Mode options: `linear`, `binary`, `both`.
* `GET /api/index`: Returns current sorted inverted index.
* `GET /api/benchmark`: Runs scaling performance benchmarks ($N = 100 \dots 100,000$).

---

## 🚢 Deployment Guide

### Option A: Standalone Docker Deployment
A single container hosting both Node.js API and C++ binary:
1. Include `g++` and `cmake` in base Docker image (`node:18-bullseye`).
2. Run `cmake --build build` during build phase.
3. Start `server.js` with static frontend built output served via Express.

### Option B: Cloud Hosting (Render / Railway / Vercel)
* **Backend API + Engine**: Deploy `backend/` & `engine/` to Render or Railway as a Web Service. Add build command `cd engine && cmake -B build -S . && cmake --build build && cd ../backend && npm install`.
* **Frontend**: Deploy `frontend/` to Vercel or Netlify with environment variable `VITE_API_URL`.

---

## 📌 Scope & Limitations
* Educational implementation focused on algorithm comparison clarity.
* Exact term matching (token level).
* Inverted index resides in memory during engine execution.
* Timings on ultra-small search queries ($N < 10$) are measured in sub-microseconds ($\mu\text{s}$) where timing variance depends on CPU hardware frequency.

---

## 📜 License
MIT License. Open source for educational and comparative research purposes.
