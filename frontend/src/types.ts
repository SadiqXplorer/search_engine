export interface SearchResult {
  found: boolean;
  query: string;
  normalizedQuery: string;
  method: string;
  comparisons: number;
  executionTimeUs: number;
  documents: string[];
}

export interface ComparisonPair {
  linear: SearchResult;
  binary: SearchResult;
}

export interface IndexEntry {
  term: string;
  documents: string[];
}

export interface IndexData {
  totalTerms: number;
  totalDocuments: number;
  entries: IndexEntry[];
}

export interface BenchmarkPoint {
  datasetSize: number;
  queryType: string;
  linearComparisons: number;
  linearTimeUs: number;
  binaryComparisons: number;
  binaryTimeUs: number;
}
