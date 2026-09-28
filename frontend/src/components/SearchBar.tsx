import React, { useState } from 'react';
import { Search, Zap, GitCompare, ListFilter } from 'lucide-react';

interface SearchBarProps {
  onSearch: (query: string, mode: 'linear' | 'binary' | 'both') => void;
  loading: boolean;
}

export const SearchBar: React.FC<SearchBarProps> = ({ onSearch, loading }) => {
  const [query, setQuery] = useState('');
  const [mode, setMode] = useState<'linear' | 'binary' | 'both'>('both');

  const handleSubmit = (e: React.FormEvent) => {
    e.preventDefault();
    if (query.trim()) {
      onSearch(query.trim(), mode);
    }
  };

  const sampleQueries = ['machine', 'database', 'algorithm', 'cpp', 'network', 'learning'];

  return (
    <div className="w-full max-w-4xl mx-auto space-y-4">
      <form onSubmit={handleSubmit} className="space-y-4">
        {/* Search input container */}
        <div className="relative flex items-center shadow-2xl rounded-2xl overflow-hidden glass-card focus-within:ring-2 focus-within:ring-cyan-500 transition-all">
          <div className="pl-5 text-slate-400">
            <Search className="w-6 h-6 text-cyan-400" />
          </div>
          <input
            type="text"
            value={query}
            onChange={(e) => setQuery(e.target.value)}
            placeholder="Search indexed terms (e.g. 'machine', 'cpp', 'database')..."
            className="w-full px-4 py-4 bg-transparent text-lg text-slate-100 placeholder-slate-500 focus:outline-none"
          />
          <button
            type="submit"
            disabled={loading || !query.trim()}
            className="mr-2 px-6 py-3 bg-gradient-to-r from-cyan-500 to-blue-600 hover:from-cyan-400 hover:to-blue-500 disabled:opacity-50 text-white font-semibold rounded-xl transition-all shadow-lg flex items-center gap-2"
          >
            {loading ? (
              <div className="w-5 h-5 border-2 border-white border-t-transparent rounded-full animate-spin" />
            ) : (
              <Search className="w-4 h-4" />
            )}
            <span>Search</span>
          </button>
        </div>

        {/* Search mode toggles */}
        <div className="flex flex-wrap items-center justify-between gap-3 pt-2">
          <div className="flex items-center gap-2 bg-slate-900/80 p-1.5 rounded-xl border border-slate-800">
            <button
              type="button"
              onClick={() => setMode('both')}
              className={`px-4 py-2 text-sm font-medium rounded-lg transition-all flex items-center gap-2 ${
                mode === 'both'
                  ? 'bg-cyan-500/20 text-cyan-300 border border-cyan-500/40 shadow-sm'
                  : 'text-slate-400 hover:text-slate-200'
              }`}
            >
              <GitCompare className="w-4 h-4" />
              <span>Compare Both</span>
            </button>
            <button
              type="button"
              onClick={() => setMode('linear')}
              className={`px-4 py-2 text-sm font-medium rounded-lg transition-all flex items-center gap-2 ${
                mode === 'linear'
                  ? 'bg-amber-500/20 text-amber-300 border border-amber-500/40 shadow-sm'
                  : 'text-slate-400 hover:text-slate-200'
              }`}
            >
              <ListFilter className="w-4 h-4" />
              <span>Linear Search O(n)</span>
            </button>
            <button
              type="button"
              onClick={() => setMode('binary')}
              className={`px-4 py-2 text-sm font-medium rounded-lg transition-all flex items-center gap-2 ${
                mode === 'binary'
                  ? 'bg-emerald-500/20 text-emerald-300 border border-emerald-500/40 shadow-sm'
                  : 'text-slate-400 hover:text-slate-200'
              }`}
            >
              <Zap className="w-4 h-4" />
              <span>Binary Search O(log n)</span>
            </button>
          </div>

          {/* Quick chip queries */}
          <div className="flex items-center gap-2 overflow-x-auto py-1">
            <span className="text-xs text-slate-500 font-medium">Try:</span>
            {sampleQueries.map((item) => (
              <button
                key={item}
                type="button"
                onClick={() => {
                  setQuery(item);
                  onSearch(item, mode);
                }}
                className="px-2.5 py-1 text-xs bg-slate-800/80 hover:bg-slate-700 text-slate-300 rounded-md border border-slate-700/60 transition-colors"
              >
                {item}
              </button>
            ))}
          </div>
        </div>
      </form>
    </div>
  );
};
